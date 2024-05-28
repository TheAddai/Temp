#pragma once

#include "pch.h"
#include "frame_buffer.h"
#include "renderer.h"

// openGL
#include "gl_frame_buffer.h"

namespace prime {

	Ref<Framebuffer> Framebuffer::Create(ui32 width, ui32 height)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::openGL:
			return CreateRef<GLFramebuffer>(width, height);
			break;
		}
		return nullptr;
	}
}