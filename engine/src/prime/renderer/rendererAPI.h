#pragma once

#include "prime/core/defines.h"

#include <glm/glm.hpp>

namespace prime {

	class RendererAPI
	{
	public:
		virtual void Init(void* windowHandle) = 0;
		virtual void Shutdown() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SwapBuffers(void* windowHandle) = 0;
		virtual void Flush() = 0;

		virtual void BeginDrawing() = 0;
		virtual void EndDrawing() = 0;
		virtual void SetViewport(ui32 width, ui32 height) = 0;

		virtual void DrawQuad(const glm::vec2& position, const glm::vec2& scale) = 0;
	};
}
