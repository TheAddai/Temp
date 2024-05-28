#pragma once

#include "pch.h"
#include "frame_buffer.h"
#include "gl_frame_buffer.h"

namespace prime {

	Ref<Framebuffer> Framebuffer::Create(ui32 width, ui32 height)
	{
		return Ref<Framebuffer>();
	}
}