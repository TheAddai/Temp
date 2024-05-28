
#include "editor.h"

#include <imgui/imgui.h>

namespace prime {

	void Editor::Init()
	{
		Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		m_scene = Scene::Create();
		m_scene->Init();

		m_frameBuffer = Framebuffer::Create(640, 480);

		Entity red = m_scene->CreateEntity();
		red.GetComponent<TransformComponent>().scale.x = 0.2f;
		red.GetComponent<TransformComponent>().scale.y = 0.2f;
		red.GetComponent<TransformComponent>().position.x = -0.4f;
		red.AddComponent<SpriteComponent>().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		Entity green = m_scene->CreateEntity();
		green.GetComponent<TransformComponent>().scale.x = 0.2f;
		green.GetComponent<TransformComponent>().scale.y = 0.2f;
		green.AddComponent<SpriteComponent>().color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		Entity blue = m_scene->CreateEntity();
		blue.GetComponent<TransformComponent>().scale.x = 0.2f;
		blue.GetComponent<TransformComponent>().scale.y = 0.2f;
		blue.GetComponent<TransformComponent>().position.x = .4f;
		blue.AddComponent<SpriteComponent>().color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	void Editor::Shutdown()
	{
		m_scene->Destroy();
	}

	void Editor::Update()
	{
		ResizeViewport();

		m_frameBuffer->Bind();
		Renderer::DrawScene(m_scene);
		m_frameBuffer->Unbind();
	}

	void Editor::ImGuiRender()
	{
		Dockspace();
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
}