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
			config.title = "Prime Editor - Untitled";
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

		void MenuBar();
		std::string GetNameFromPath(const std::string& path);

		void SaveSceneAs();
		void OpenScene();
		void NewScene();
		void SaveScene();

		void OnKeyPressed(const KeyPressedEvent& e);
		void SetShourcus(Key key);

	private:
		Ref<Scene> m_scene;
		Ref<Framebuffer> m_frameBuffer;
		glm::vec2 m_viewportSize = glm::vec2(0.0f);
		std::string m_sceneSavePath = "";

		EditorCamera m_editorCamera;

		// panels
		SceneHeirarchy m_sceneHeirarchy;
		Properties m_properties;
	};
}