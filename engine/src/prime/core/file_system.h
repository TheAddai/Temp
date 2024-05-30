#pragma once

#include "prime/scene/scene.h"

#include <string>

namespace prime {

	class FileSystem
	{
	public:
		static void SaveScene(Ref<Scene>& scene, const std::string& path, const std::string& name);
		static b8 LoadScene(Ref<Scene>& scene, const std::string& path);
	};
}
