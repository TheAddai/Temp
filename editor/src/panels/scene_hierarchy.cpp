
#include "scene_hierarchy.h"
#include "prime/scene/components.h"

#include <imgui/imgui.h>

namespace prime {

	void SceneHeirarchy::SetScene(Ref<Scene>& scene, b8 loaded)
	{
		m_sceneLoaded = loaded;
		m_entities.clear();
		m_scene = scene;
		m_selectedEntity = {};
	}

	void SceneHeirarchy::ImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_scene)
		{
			m_entities.clear();

			entt::basic_view entities = m_scene->m_registry.view<TransformComponent>();
			for (entt::entity entityID : entities)
			{
				Entity entity{ entityID, m_scene.get()};
				m_entities.push_back(entity);
			}

			if (m_sceneLoaded)
			{
				for (auto entity : m_entities)
				{
					DrawEntityNode(entity);
				}
			}
			else if (!m_sceneLoaded)
			{
				// draw entities in reverse
			    for (auto it = m_entities.rbegin(); it != m_entities.rend(); ++it)
			    {
				    DrawEntityNode(*it);
			    }
			}

			

			

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_selectedEntity = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Entity"))
					m_scene->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHeirarchy::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().name;

		ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		bool opened = ImGui::TreeNodeEx((void*)(ui64)(ui32)entity, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		b8 entityDeleted = false;
		b8 entityDuplicated = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity")) { entityDeleted = true; }
			if (ImGui::MenuItem("Duplicate Entity")) { entityDuplicated = true; }

			ImGui::EndPopup();
		}

		if (opened)
		{
			// add child nodes here
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_scene->DestroyEntity(entity);
			if (m_selectedEntity == entity)
				m_selectedEntity = {};
		}

		if (entityDuplicated)
		{
			m_scene->DuplicateEntity(entity);
		}
	}
}