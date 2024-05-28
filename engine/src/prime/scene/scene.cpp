
#include "pch.h"
#include "scene.h"
#include "entity.h"
#include "components.h"
#include "prime/renderer/renderer.h"

namespace prime {

	Scene::~Scene()
	{
		entt::basic_view entities = m_registry.view<TransformComponent>();
		for (entt::entity entity : entities)
		{
			m_registry.destroy(entity);
		}
	}

	Entity Scene::CreateEntity()
	{
		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<IDComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::Draw()
	{
		Renderer::Clear();
		Renderer::BeginDrawing();

		entt::basic_view sEs = m_registry.view<TransformComponent, SpriteComponent>();
		for (entt::entity sE : sEs)
		{
			auto [sT, s] = sEs.get<TransformComponent, SpriteComponent>(sE);
			Renderer::DrawQuad(sT.position, sT.scale, s.color, sT.rotation);
		}

		Renderer::EndDrawing();
	}

	Ref<Scene> Scene::Create()
	{
		return CreateRef<Scene>();
	}
}