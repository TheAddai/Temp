
#include "environment_panel.h"
#include "prime/renderer/renderer.h"

#include <imgui/imgui.h>

namespace prime {

	void EnvironmentPanel::SetScene(Ref<Scene>& scene)
	{
		m_scene = scene;
	}

	void EnvironmentPanel::Update(EditorCamera& camera)
	{
		if (m_showColliders)
		{
			Renderer::DrawPhysicsColliders(m_scene, camera);
		}
	}

	void prime::EnvironmentPanel::ImGuiRender()
	{
		ImGui::Begin("Environment");
		ImGui::Checkbox("Show Physics Colliders", &m_showColliders);
		ImGui::End();
	}
}