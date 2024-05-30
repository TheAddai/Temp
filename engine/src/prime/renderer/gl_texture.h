#pragma once

#include "texture.h"

namespace prime {

	class GLTexture : public Texture
	{
	public:
		GLTexture(ui32 width, ui32 height, TextureFilter filter, TextureWrap wrap);
		GLTexture(const std::string& path);
		~GLTexture();

		ui32 GetID() const override { return m_handle; }
		std::string GetPath() const override { return m_path; }

		b8 operator==(const Texture& other) const override
		{
			return m_handle == other.GetID();
		}

	private:
		void Generate(void* data);

	private:
		ui32 m_handle = 0;
		ui32 m_width = 0, m_height = 0;
		TextureFilter m_filter = TextureFilter::linear;
		TextureWrap m_wrap = TextureWrap::repeat;

		std::string m_path = "";
	};
}