#pragma once

#include "prime/scene/scene.h"
#include "prime/scene/entity.h"

#include <vector>

namespace prime {

	class SceneHeirarchy
	{
	public:
		void SetScene(Ref<Scene>& scene, b8 loaded = false);
		void ImGuiRender();

		Entity GetSelectedEntity() const { return m_selectedEntity; }

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_scene;
		Entity m_selectedEntity;

		std::vector<Entity> m_entities;
		b8 m_sceneLoaded = false;
	};
}
