
#include "scene_hierarchy.h"
#include "prime/scene/components.h"

#include <imgui/imgui.h>

namespace prime {

	void SceneHeirarchy::SetScene(Ref<Scene>& scene)
	{
		m_scene = scene;
		m_selectedEntity = {};
	}

	void SceneHeirarchy::ImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_scene)
		{
			entt::basic_view entities = m_scene->m_registry.view<TransformComponent>();
			ui64 maxEntities = entities.size() - 1;

			for (entt::entity entityID : entities)
			{
				entt::entity id = (entt::entity)(maxEntities - (ui32)entityID);
				Entity entity{ id , m_scene.get()};
				DrawEntityNode(entity);
			}
		}

		ImGui::End();
	}

	void SceneHeirarchy::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().name;

		ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		if (opened)
		{
			// add child nodes here
			ImGui::TreePop();
		}
	}
}