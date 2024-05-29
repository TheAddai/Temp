
#include "properties.h"
#include "prime/scene/components.h"

#include <imgui/imgui.h>

namespace prime {

	template<typename T>
	void DisplayAddComponentEntry(Entity entity, const std::string& entryName)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void Properties::ImGuiRender(Entity entity)
	{
		ImGui::Begin("Properties");
		m_selection = entity;

		if (m_selection)
		{
			DrawNameComponent();
			DrawAddComponentButton();
		}
		ImGui::End();
	}

	void Properties::DrawNameComponent()
	{
		auto& name = m_selection.GetComponent<NameComponent>().name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Tag", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll))
		{
			name = std::string(buffer);
		}
	}
	
	void Properties::DrawAddComponentButton()
	{
		f32 width = ImGui::GetContentRegionAvailWidth();
		ImGui::SetCursorPos({width / 2.0f - (100.0f / 2.0f), ImGui::GetCursorPosY() + 30.0f});

		if (ImGui::Button("Add Component", {100.0f, 20.0f}))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<SpriteComponent>(m_selection, "Sprite Renderer");
			DisplayAddComponentEntry<CameraComponent>(m_selection, "Camera");
			ImGui::EndPopup();
		}
	}
}