#pragma once

#include "frame_buffer.h"

namespace prime {

	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(ui32 width, ui32 height);
		~GLFramebuffer() override;

		void Resize(ui32 width, ui32 height) override;

		void Bind() override;
		void Unbind() override;

		ui32 GetHandleID() const override { return m_handle; }
		ui32 GetTextureID() override { return m_texture; }

		ui32 GetWidth() const override { return m_width; }
		ui32 GetHeight() const override { return m_height; }

	private:
		void Generate();

	private:
		ui32 m_width = 0, m_height = 0, m_renderBuffer = 0;
		ui32 m_handle = 0, m_texture = 0;
	};
}