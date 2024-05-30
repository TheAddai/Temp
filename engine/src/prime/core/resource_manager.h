#pragma once

#include "prime/renderer/texture.h"

namespace prime {

	class ResourceManager
	{
	public:
		static Ref<Texture> LoadTexture(const std::string& path);
	};
}
