#pragma once

#include "prime/core/ref.h"

#include <entt/entt.hpp>

namespace prime {

	class Entity;
	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity& entity);

		void Draw();

		static Ref<Scene> Create();

	private:
		entt::registry m_registry;

		friend class Entity;
	};
}
