#pragma once

#include "prime/core/defines.h"
#include "prime/core/ref.h"

#include <entt/entt.hpp>
#include <map>
#include <string>

namespace prime {

	class Entity;
	class Scene
	{
	public:
		Scene() = default;

		void Destroy();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity& entity);

		static Ref<Scene> Create();

	private:
		entt::registry m_registry;
		std::map<ui64, Entity> m_entities;

		friend class Entity;
		friend class Renderer;
		friend class SceneHeirarchy;
		friend class FileSystem;
	};
}
