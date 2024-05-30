
#include "pch.h"
#include "gl_texture.h"

#include <glad/glad.h>

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

		Generate();
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &m_handle);
	}

	void GLTexture::Generate()
	{
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);
		SetTextureFilter(m_filter);
		SetTextureWrap(m_wrap);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &s_whiteData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
