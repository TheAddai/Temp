#pragma once

#include "prime/core/defines.h"
#include "prime/core/ref.h"

namespace prime {

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Resize(ui32 width, ui32 height) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual ui32 GetHandleID() const = 0;
		virtual ui32 GetTextureID() = 0;

		virtual ui32 GetWidth() const = 0;
		virtual ui32 GetHeight() const = 0;

		static Ref<Framebuffer> Create(ui32 width, ui32 height);
	};
}
