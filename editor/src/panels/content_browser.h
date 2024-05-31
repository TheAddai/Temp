#pragma once

#include "prime/renderer/texture.h"

#include <filesystem>

namespace prime {

	class ContentBrowser
	{
	public:
		ContentBrowser() = default;

		void Init();
		void OnImGuiRender();

	private:
		std::filesystem::path m_baseDirectory;
		std::filesystem::path m_currentDirectory;

		Ref<Texture> m_fileIcon;
		Ref<Texture> m_folderIcon;
	};
}
