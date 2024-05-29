#pragma once

#include "prime/prime.h"
#include "panels/scene_hierarchy.h"
#include "panels/properties.h"

namespace prime {

	class Editor : public Game
	{
	public:
		void Init() override;
		void Shutdown() override;

		void Update() override;
		void ImGuiRender() override;

		Config GetConfig() const override
		{
			Config config;
			config.fullScreen = false;
			config.title = "Prime Editor";
			config.vSync = true;
			config.width = 1000;
			config.height = 600;

			return config;
		}

	private:
		void Dockspace();
		void Viewport();

		void ResizeViewport();
		void DarkTheme();

	private:
		Ref<Scene> m_scene;
		Ref<Framebuffer> m_frameBuffer;
		glm::vec2 m_viewportSize = glm::vec2(0.0f);

		// panels
		SceneHeirarchy m_sceneHeirarchy;
		Properties m_properties;
	};
}