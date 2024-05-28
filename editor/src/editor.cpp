
#include "editor.h"

#include <imgui/imgui.h>

namespace prime {

	void Editor::Init()
	{
		Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		m_scene = Scene::Create();

		Entity red = m_scene->CreateEntity();
		red.GetComponent<TransformComponent>().scale.x = 0.2f;
		red.GetComponent<TransformComponent>().scale.y = 0.2f;
		red.GetComponent<TransformComponent>().position.x = -.4f;
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

	}

	void Editor::Update()
	{
		m_scene->Draw();
	}

	void Editor::ImGuiRender()
	{
		static b8 show = true;
		ImGui::ShowDemoWindow(&show);
	}
}