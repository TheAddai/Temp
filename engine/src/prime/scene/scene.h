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

		void Init();
		void Destroy();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity& entity);

		Entity& GetMainCamera();
		void SetMainCamera(Entity& entity);

		static Ref<Scene> Create();

	private:
		entt::registry m_registry;

		ui64 m_cameraEntityGUID = 0;
		std::map<ui64, Entity> m_entities;

		friend class Entity;
		friend class Renderer;
		friend class SceneHeirarchy;
	};
}
