#pragma once

#include "rendererAPI.h"

namespace prime {

	class GLRendererAPI : public RendererAPI
	{
	public:
		GLRendererAPI() = default;

		void Init(void* windowHandle) override;
		void Shutdown() override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void SwapBuffers(void* windowHandle) override;
		void Flush() override;

		void BeginDrawing(const glm::mat4& projection) override;
		void EndDrawing() override;
		void SetViewport(ui32 width, ui32 height) override;

		// Primitives
		void DrawQuad(Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, f32 rotation) override;
		void DrawLine(const glm::vec2& point, const glm::vec2& point2, const glm::vec4& color) override;
		void DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, f32 rotation) override;

	private:
		void InitSpriteRendering();
		void InitLineRendering();

		void StartBatch();
		f32 GetTextureIndex(Ref<Texture>& texture);
	};
}
