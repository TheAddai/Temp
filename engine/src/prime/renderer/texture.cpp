
#include "pch.h"
#include "texture.h"
#include "renderer.h"

// openGL
#include "gl_texture.h"

namespace prime {

	Ref<Texture> Texture::Create(ui32 width, ui32 height, TextureFilter filter, TextureWrap wrap)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			return CreateRef<GLTexture>(width, height, filter, wrap);
			break;
		}
		return nullptr;
	}

	Ref<Texture> Texture::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			return CreateRef<GLTexture>(path);
			break;
		}
		return nullptr;
	}
}