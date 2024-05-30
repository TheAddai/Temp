#pragma once

#include "texture.h"

namespace prime {

	class GLTexture : public Texture
	{
	public:
		GLTexture(ui32 width, ui32 height, TextureFilter filter, TextureWrap wrap);
		~GLTexture();

		ui32 GetID() const override { return m_handle; }

	private:
		void Generate();

	private:
		ui32 m_handle = 0;
		ui32 m_width = 0, m_height = 0;
		TextureFilter m_filter;
		TextureWrap m_wrap;
	};
}