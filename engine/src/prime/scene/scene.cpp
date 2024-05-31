
#include "pch.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "prime/renderer/renderer.h"

namespace prime {

	template<typename Component>
	static void CopyComponent(Entity newEntity, Entity oldEntity)
	{
		if (oldEntity.HasComponent<Component>())
		{
			newEntity.AddOrReplaceComponent<Component>(oldEntity.GetComponent<Component>());
		}
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, std::unordered_map<ui64, entt::entity>& enttMap)
	{
		auto view = srcRegistry.view<Component>();
		for (auto srcEntity : view)
		{
			entt::entity dstEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).guid);
			auto& srcComponent = srcRegistry.get<Component>(srcEntity);
			dstRegistry.emplace_or_replace<Component>(dstEntity, srcComponent);
		}
	}

	Scene::~Scene()
	{
		entt::basic_view entities = m_registry.view<TransformComponent>();
		for (entt::entity entity : entities)
		{
			m_registry.destroy(entity);
		}
		m_entities.clear();
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		auto& srcSceneRegistry = other->m_registry;
		auto& dstSceneRegistry = newScene->m_registry;
		std::unordered_map<ui64, entt::entity> enttMap;

		// Create entities in new scene
		auto entities = srcSceneRegistry.view<IDComponent>();
		for (entt::entity entity : entities)
		{
			Guid uuid = srcSceneRegistry.get<IDComponent>(entity).guid;
			const auto& name = srcSceneRegistry.get<NameComponent>(entity).name;
			Entity newEntity = newScene->CreateEntityWithGuid(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<RectComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<LineComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<RigidbodyComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxColliderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithGuid(Guid(), name);
	}

	Entity Scene::CreateEntityWithGuid(Guid guid, const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
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

	Entity Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetComponent<NameComponent>().name;
		Entity newEntity = CreateEntity(name);
		CopyComponent<TransformComponent>(newEntity, entity);
		CopyComponent<SpriteComponent>(newEntity, entity);
		CopyComponent<CameraComponent>(newEntity, entity);
		CopyComponent<RectComponent>(newEntity, entity);
		CopyComponent<LineComponent>(newEntity, entity);

		return newEntity;
	}

	Entity Scene::GetMainCamera()
	{
		entt::basic_view cEs = m_registry.view<CameraComponent>();
		for (entt::entity cE : cEs)
		{
			CameraComponent camera = cEs.get<CameraComponent>(cE);
			if (camera.primary)
			{
				return Entity(cE, this);
			}
		}
		return Entity();
	}

	Ref<Scene> Scene::Create()
	{
		return CreateRef<Scene>();
	}
}