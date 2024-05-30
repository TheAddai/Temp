
#include "editor.h"

#include <imgui/imgui.h>

namespace prime {

	void Editor::Init()
	{
		DarkTheme();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		f32 fontSize = 14.0f;
		io.Fonts->AddFontFromFileTTF("assets/fonts/roboto_bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/roboto_regular.ttf", fontSize);

		Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		m_scene = Scene::Create();

		m_frameBuffer = Framebuffer::Create(640, 480);
		m_sceneHeirarchy.SetScene(m_scene);

		Dispatcher::Get().sink<KeyPressedEvent>().connect<&Editor::OnKeyPressed>(this);
		m_editorCamera.SubscribeToEvent();
	}

	void Editor::Shutdown()
	{
		m_scene->Destroy();
	}

	void Editor::Update()
	{
		ResizeViewport();

		m_editorCamera.Update();

		m_frameBuffer->Bind();
		Renderer::DrawSceneEditor(m_scene, m_editorCamera);
		m_frameBuffer->Unbind();
	}

	void Editor::ImGuiRender()
	{
		Dockspace();
		m_sceneHeirarchy.ImGuiRender();
		m_properties.ImGuiRender(m_sceneHeirarchy.GetSelectedEntity());
		Viewport();
	}
	
	void Editor::Dockspace()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		MenuBar();

		ImGui::End();
	}
	
	void Editor::Viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_frameBuffer->GetTextureID();
		ImGui::Image((ImTextureID)textureID, { m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}
	
	void Editor::ResizeViewport()
	{
		glm::vec2 size = glm::vec2(0.0f);
		size.x = (f32)m_frameBuffer->GetWidth();
		size.y = (f32)m_frameBuffer->GetHeight();

		if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(size.x != m_viewportSize.x || size.y != m_viewportSize.y))
		{
			m_frameBuffer->Resize((ui32)m_viewportSize.x, (ui32)m_viewportSize.y);
		}
	}

	void Editor::DarkTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
	
	void Editor::MenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
				{
					OpenScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveScene();
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Engine::Exit();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	std::string Editor::GetNameFromPath(const std::string& path)
	{
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

		return path.substr(lastSlash, count);
	}

	void Editor::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Prime Scene (*.prime)\0*.prime\0");
		if (!filepath.empty())
		{
			std::string name = GetNameFromPath(filepath);
			FileSystem::SaveScene(m_scene, filepath, name);
			std::string title = "Prime Engine - " + name;
			Engine::SetTitle(title);
			m_sceneSavePath = filepath;
		}
	}

	void Editor::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Prime Scene (*.prime)\0*.prime\0");
		Ref<Scene> newScene = Scene::Create();
		if (FileSystem::LoadScene(newScene, filepath))
		{
			std::string name = GetNameFromPath(filepath);
			m_scene = newScene;
			m_sceneHeirarchy.SetScene(m_scene, true);

			std::string title = "Prime Engine - " + name;
			Engine::SetTitle(title);
			m_sceneSavePath = filepath;
		}
	}
	
	void Editor::NewScene()
	{
		m_scene = Scene::Create();
		m_sceneHeirarchy.SetScene(m_scene);
		m_sceneSavePath = "";
	}
	
	void Editor::SaveScene()
	{
		if (!m_sceneSavePath.empty())
		{
			std::string name = GetNameFromPath(m_sceneSavePath);
			FileSystem::SaveScene(m_scene, m_sceneSavePath, name);
			std::string title = "Prime Engine - " + name;
			Engine::SetTitle(title);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void Editor::OnKeyPressed(const KeyPressedEvent& e)
	{
		if (e.GetRepeat()) { return; }
		SetShourcus(e.GetKey());
	}

	void Editor::SetShourcus(Key key)
	{
		bool control = Input::IsKeyPressed(Key::keyLeftControl) || Input::IsKeyPressed(Key::keyLeftControl);
		bool shift = Input::IsKeyPressed(Key::keyLeftShift) || Input::IsKeyPressed(Key::keyRightShift);

		switch (key)
		{
		case Key::keyN:
		{
			if (control) { NewScene(); }
			break;
		}

		case Key::keyO:
		{
			if (control) { OpenScene(); }
			break;
		}

		case Key::keyS:
		{
			if (control)
			{
				if (shift) { SaveSceneAs(); }
				else { SaveScene(); }
			}
			break;
		}

		case Key::keyD:
		{
			if (control) { m_scene->DuplicateEntity(m_sceneHeirarchy.GetSelectedEntity()); }
			break;
		}

		}
	}
}