#pragma once

namespace prime {

	class RendererAPI
	{
	public:
		virtual void Init(void* windowHandle) = 0;
		virtual void Shutdown() = 0;

		virtual void SetClearColor() = 0;
		virtual void Clear() = 0;

		virtual void SwapBuffers(void* windowHandle) = 0;
	};
}
