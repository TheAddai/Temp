
#include "pch.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "prime/renderer/renderer.h"

namespace prime {

	void Scene::Init()
	{
		m_entities.clear();
		m_cameraEntityGUID = 0;

		Entity mainCamera = CreateEntity("MainCamera");
		mainCamera.AddComponent<CameraComponent>();
		m_cameraEntityGUID = mainCamera.GetComponent<IDComponent>().guid;
	}

	void Scene::Destroy()
	{
		entt::basic_view entities = m_registry.view<TransformComponent>();
		for (entt::entity entity : entities)
		{
			m_registry.destroy(entity);
		}

		m_cameraEntityGUID = 0;
		m_entities.clear();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Guid guid = Guid();
		Entity entity = Entity(m_registry.create(), this);
		entity.AddComponent<IDComponent>(guid);
		entity.AddComponent<TransformComponent>();
		auto& nameC = entity.AddComponent<NameComponent>();
		nameC.name = name.empty() ? "Entity" : name;

		m_entities[guid] = entity;
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		ui64 guid = entity.GetComponent<IDComponent>().guid;
		m_registry.destroy(entity);
		m_entities.erase(guid);
	}

	Entity& Scene::GetMainCamera()
	{
		return m_entities[m_cameraEntityGUID];
	}

	void Scene::SetMainCamera(Entity& entity)
	{
		if (entity.HasComponent<CameraComponent>())
		{
			ui64 guid = entity.GetComponent<IDComponent>().guid;
			m_cameraEntityGUID = guid;
		}
	}

	Ref<Scene> Scene::Create()
	{
		return CreateRef<Scene>();
	}
}