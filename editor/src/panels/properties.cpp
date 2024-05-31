
#include "properties.h"
#include "prime/scene/components.h"
#include "UI.h"
#include "prime/renderer/renderer.h"
#include "prime/core/resource_manager.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

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

			DrawComponent<TransformComponent>("Transform", entity, false, [](auto& component)
				{
					f32 columnWidth = 70.0f;
					DrawVec2Control("Position", component.position, 0.0f, columnWidth);
					DrawVec2Control("Scale", component.scale, 1.0f, columnWidth);

					f32 rotation = glm::degrees(component.rotation);
					DrawfloatControl("Rotation", rotation, 0.0f, columnWidth);
					component.rotation = glm::radians(rotation);
				});

			DrawComponent<SpriteComponent>("Sprite", entity, true, [](auto& component)
				{
					ImGui::ColorPicker4("##SpriteColor", glm::value_ptr(component.color));
					ui64 textureID = 0;
					
					if (component.texture.get())
					{
						textureID = component.texture->GetID();
						ImGui::ImageButton((ImTextureID)textureID, { 20.0f, 20.0f }, { 0, 1 }, { 1, 0 });
					}
					else
					{
						textureID = Renderer::GetDefaultTexture()->GetID();
						ImGui::ImageButton((ImTextureID)textureID, { 20.0f, 20.0f }, { 0, 1 }, { 1, 0 });
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath(path);
							Ref<Texture> texture = ResourceManager::LoadTexture(texturePath.string());
							if (texture.get())
							{
								component.texture = texture;
							}
							else 
							{    P_ERROR("Could not load texture {0}", texturePath.filename().string()); }
						}
						ImGui::EndDragDropTarget();
					}

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Remove Texture")) 
						{ 
							component.texture.reset();
						}
						ImGui::EndPopup();
					}
				});

			DrawComponent<CameraComponent>("Camera", entity, true, [](auto& component)
				{
					ImGui::ColorPicker4("##CameraColor", glm::value_ptr(component.clearColor));

					Camera& camera = component.camera;
					ImGui::Checkbox("Primary", &component.primary);

					float orthoSize = camera.GetSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetSize(orthoSize);

					float orthoNear = camera.GetNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetNearClip(orthoNear);

					float orthoFar = camera.GetFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetFarClip(orthoFar);
				});

			DrawComponent<LineComponent>("Line", entity, true, [](auto& component)
				{
					DrawVec2Control("End", component.endPosition, 1.0f, 70.0f);
					ImGui::NewLine();
					ImGui::ColorPicker4("##LineColor", glm::value_ptr(component.color));
				});

			DrawComponent<RectComponent>("Rect", entity, true, [](auto& component)
				{
					ImGui::ColorPicker4("##RectColor", glm::value_ptr(component.color));
				});

			DrawComponent<RigidbodyComponent>("Rigidbody", entity, true, [](auto& component)
				{
					const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
					const char* currentBodyTypeString = bodyTypeStrings[(int)component.type];
					if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
					{
						for (int i = 0; i < 3; i++)
						{
							//P_INFO(i);
							bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
							if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
							{
								currentBodyTypeString = bodyTypeStrings[i];
								component.type = (BodyType)i;
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);

					ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
				});

			DrawComponent<BoxColliderComponent>("BoxCollider", entity, true, [](auto& component)
				{
					ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
					ImGui::DragFloat2("Size", glm::value_ptr(component.size));
				});

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
			DisplayAddComponentEntry<SpriteComponent>(m_selection, "Sprite");
			DisplayAddComponentEntry<CameraComponent>(m_selection, "Camera");
			DisplayAddComponentEntry<LineComponent>(m_selection, "Line");
			DisplayAddComponentEntry<RectComponent>(m_selection, "Rect");
			DisplayAddComponentEntry<RigidbodyComponent>(m_selection, "Rigidbody");
			DisplayAddComponentEntry<BoxColliderComponent>(m_selection, "BoxCollider");
			ImGui::EndPopup();
		}
	}
}