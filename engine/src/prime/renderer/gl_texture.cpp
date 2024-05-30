
#include "pch.h"
#include "gl_texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace prime {

	static ui32 s_whiteData = 0xffffffff;

	static void SetTextureFilter(TextureFilter filter)
	{
		switch (filter)
		{
		case TextureFilter::nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TextureFilter::linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
	}

	static void SetTextureWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TextureWrap::clamp:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		}
	}

	GLTexture::GLTexture(ui32 width, ui32 height, TextureFilter filter, TextureWrap wrap)
	{
		m_width = 1;
		m_height = 1;
		m_filter = filter;
		m_wrap = wrap;

		Generate(nullptr);
	}

	GLTexture::GLTexture(const std::string& path)
	{
		i32 width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			m_path = path;
			m_width = width;
			m_height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_width = width;
			m_height = height;
			m_filter = TextureFilter::linear;
			m_wrap = TextureWrap::repeat;

			Generate(data);
			stbi_image_free(data);
		}
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &m_handle);
	}

	void GLTexture::Generate(void* data)
	{
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);
		SetTextureFilter(m_filter);
		SetTextureWrap(m_wrap);

		if (data == nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &s_whiteData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
