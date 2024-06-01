#pragma once

#include "prime/prime.h"

#include "panels/scene_hierarchy.h"
#include "panels/properties.h"
#include "panels/content_browser.h"
#include "panels/renderer_panel.h"
#include "panels/environment_panel.h"

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
			config.fullScreen = true;
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
		void OpenScene(const std::filesystem::path& path);
		void NewScene();
		void SaveScene();

		void OnKeyPressed(const KeyPressedEvent& e);
		void SetShourcus(Key key);

		void DrawPlayAndStopButton();
		void DuplicateEntity();

		void SceneEdit();
		void ScenePlay();


	private:
		Ref<Scene> m_scene;
		Ref<Scene> m_editorScene;

		Ref<Framebuffer> m_frameBuffer;
		glm::vec2 m_viewportSize = glm::vec2(0.0f);
		std::string m_sceneSavePath = "";

		EditorCamera m_editorCamera;

		// panels
		SceneHeirarchy m_sceneHeirarchy;
		Properties m_properties;
		ContentBrowser m_contextBrowser;
		RendererPanel m_rendererPanel;
		EnvironmentPanel m_environmentPanel;

		b8 m_viewportFocused = false, m_viewportHovered = false;
		glm::vec2 m_viewportBounds[2]{};

		// textures
		Ref<Texture> m_playButton;
		Ref<Texture> m_stopButton;

		enum class State
		{
			edit = 0, play = 1
		};

		State m_state = State::edit;

		// physics
		PhysicsWorld m_world;
	};
}