
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

	void Scene::Destroy()
	{
		entt::basic_view entities = m_registry.view<TransformComponent>();
		for (entt::entity entity : entities)
		{
			m_registry.destroy(entity);
		}
		m_entities.clear();
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

	Ref<Scene> Scene::Create()
	{
		return CreateRef<Scene>();
	}
}