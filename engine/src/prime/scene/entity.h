#pragma once

#include "scene.h"
#include "prime/core/assert.h"

namespace prime {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_handle(handle), m_scene(scene) {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			P_ASSERT_MSG(!HasComponent<T>(), "entity already has component!");
			T& component = m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_scene->m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			P_ASSERT_MSG(HasComponent<T>(), "entity does not have component!");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.has<T>(m_handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			P_ASSERT_MSG(HasComponent<T>(), "entity does not have component!");
			m_scene->m_registry.remove<T>(m_handle);
		}

		operator bool() const { return m_handle != entt::null; }
		operator entt::entity() const { return m_handle; }
		operator ui32() const { return (ui32)m_handle; }

		bool operator==(const Entity& other) const
		{
			return m_handle == other.m_handle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		entt::entity GetID() const { return m_handle; }

	private:
		entt::entity m_handle = entt::null;
		Scene* m_scene = nullptr;
	};
}