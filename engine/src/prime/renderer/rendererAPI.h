#pragma once

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
	};
}
