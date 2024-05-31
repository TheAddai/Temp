#pragma once

#include "prime/core/defines.h"
#include "texture.h"
#include "renderer.h"

#include <glm/glm.hpp>

namespace prime {

	class RendererAPI
	{
	public:
		virtual void Init(void* windowHandle, Renderer::Info& info) = 0;
		virtual void Shutdown() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SwapBuffers(void* windowHandle) = 0;
		virtual void Flush() = 0;

		virtual void BeginDrawing(const glm::mat4& projection) = 0;
		virtual void EndDrawing() = 0;
		virtual void SetViewport(ui32 width, ui32 height) = 0;

		virtual Ref<Texture> GetDefaultTexture() = 0;

		// Primitives
		virtual void DrawQuad(Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, f32 rotation) = 0;
		virtual void DrawLine(const glm::vec2& point, const glm::vec2& point2, const glm::vec4& color) = 0;
		virtual void DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, f32 rotation) = 0;
	};
}
