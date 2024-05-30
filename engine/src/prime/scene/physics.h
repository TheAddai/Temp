#pragma once

#include "scene.h"

#include <map>

namespace prime {

	class PhysicsWorld
	{
	public:
		void SetScene(Scene* scene);
		void Destroy();
		
		void Step(f32 deltaTime);

	private:
		void* m_world = nullptr;
		Scene* m_scene;
		std::map<ui64, void*> m_bodies;
	};
}
