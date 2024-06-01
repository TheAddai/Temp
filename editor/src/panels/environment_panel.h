#pragma once

#include "prime/core/defines.h"
#include "prime/scene/scene.h"
#include "prime/renderer/editor_camera.h"

namespace prime {

	class EnvironmentPanel
	{
	public:
		void SetScene(Ref<Scene>& scene);
		void Update(EditorCamera& camera);
		void ImGuiRender();

	private:
		Ref<Scene> m_scene;
		b8 m_showColliders = false;
	};
}
