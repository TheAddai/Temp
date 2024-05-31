
#include "pch.h"
#include "content_browser.h"
#include "prime/core/resource_manager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace prime {

	void ContentBrowser::Init()
	{
		m_baseDirectory = "assets";
		m_currentDirectory = m_baseDirectory;

		m_fileIcon = ResourceManager::LoadTexture("resources/file.png");
		m_folderIcon = ResourceManager::LoadTexture("resources/folder.png");
	}

	void ContentBrowser::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_currentDirectory != std::filesystem::path(m_baseDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		static f32 padding = 16.0f;
		static f32 thumbnailSize = 90.0f;
		f32 cellSize = thumbnailSize + padding;

		f32 panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture> icon = directoryEntry.is_directory() ? m_folderIcon : m_fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ui64 textureID = icon->GetID();
			ImGui::ImageButton((ImTextureID)textureID, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_currentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		ImGui::End();
	}
}