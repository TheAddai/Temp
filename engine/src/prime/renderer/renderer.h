#pragma once

#include "prime/core/defines.h"

#include <glm/glm.hpp>

namespace prime {

	class Window;
	class Renderer
	{
	public:
		struct Config
		{
			ui32 maxSprites = 20;
		};

		enum class API
		{
			openGL
		};

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void BeginDrawing();
		static void EndDrawing();

		// primitives
		static void DrawQuad(const glm::vec2 position, const glm::vec2 scale);

		static API GetAPI() { return s_API; }
		static Config GetConfig() { return s_config; }

	private:
		static void Init(void* windowHandle);
		static void Shutdown();
		static void SwapBuffers();

	private:
		static API s_API;
		static Config s_config;

		friend class Engine;
	};
}