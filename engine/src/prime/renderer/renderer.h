#pragma once

#include <glm/glm.hpp>

namespace prime {

	class Window;
	class Renderer
	{
	public:
		enum class API
		{
			openGL
		};

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static API GetAPI() { return s_API; }

	private:
		static void Init(void* windowHandle);
		static void Shutdown();
		static void SwapBuffers();

	private:
		static API s_API;
		friend class Engine;
	};
}