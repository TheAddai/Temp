
#include "editor.h"

namespace prime {

	void Editor::Init()
	{
		Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		m_scene = Scene::Create();

		Entity entity = m_scene->CreateEntity();
		entity.GetComponent<TransformComponent>().scale.x = 0.2f;
		entity.GetComponent<TransformComponent>().scale.y = 0.2f;
		entity.GetComponent<TransformComponent>().position.x = -.4f;
		entity.AddComponent<SpriteComponent>();
	}

	void Editor::Shutdown()
	{

	}

	void Editor::Update()
	{
		m_scene->Draw();
	}
}