
#include "scene.h"

namespace prime {

	Scene::~Scene()
	{

	}

	std::shared_ptr<Scene> Scene::Create()
	{
		return std::make_shared<Scene>();
	}
}