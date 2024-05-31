
#include "renderer_panel.h"
#include "prime/renderer/renderer.h"

#include <imgui/imgui.h>

namespace prime {

	void RendererPanel::ImGuiRender()
	{
		ImGui::Begin("Renderer");
		Renderer::Info info = Renderer::GetInfo();
		Renderer::Config config = Renderer::GetConfig();

		std::string rendererName = std::string("RendererAPI: ") + info.name;
		std::string vendor = std::string("Vendor: ") + info.vendor;
		std::string card = std::string("Graphics Card: ") + info.card;
		std::string version = std::string("Version: ") + info.version;

		std::string maxSprites = std::string("Max Sprites Per Draw Call: ") + std::to_string(config.maxSprites);

		ImGui::Text("%s", rendererName.c_str());
		ImGui::Text("%s", vendor.c_str());
		ImGui::Text("%s", card.c_str());
		ImGui::Text("%s", version.c_str());
		ImGui::Text("%s", maxSprites.c_str());

		ImGui::End();
	}
}