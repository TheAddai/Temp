#pragma once

#include "prime/scene/scene.h"
#include "prime/scene/entity.h"

namespace prime {

	class SceneHeirarchy
	{
	public:
		void SetScene(Ref<Scene>& scene);
		void ImGuiRender();

		Entity GetSelectedEntity() const { return m_selectedEntity; }

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_scene;
		Entity m_selectedEntity;
	};
}
