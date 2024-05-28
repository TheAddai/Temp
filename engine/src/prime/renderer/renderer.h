#pragma once

#include "prime/core/defines.h"
#include "prime/core/ref.h"

#include <glm/glm.hpp>

namespace prime {

	class Window;
	class Scene;
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


		static void SetViewport(ui32 width, ui32 height);
		static void DrawScene(Ref<Scene>& scene);

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