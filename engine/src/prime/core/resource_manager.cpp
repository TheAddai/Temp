
#include "pch.h"
#include "resource_manager.h"

namespace prime {

	static std::map<std::string, Ref<Texture>> s_textures;

	static b8 TextureExists(const std::string& path)
	{
		return s_textures.find(path) != s_textures.end();
	}

	Ref<Texture> ResourceManager::LoadTexture(const std::string& path)
	{
		if (TextureExists(path)) { return s_textures[path]; }
		else
		{
			Ref<Texture> texture = Texture::Create(path);
			s_textures[path] = texture;
			return texture;
		}
	}
}