#pragma once

#include "rendererAPI.h"

namespace prime {

	class GLRendererAPI : public RendererAPI
	{
	public:
		GLRendererAPI() = default;

		void Init(void* windowHandle) override;
		void Shutdown() override;

		void SetClearColor() override;
		void Clear() override;

		void SwapBuffers(void* windowHandle) override;
	};
}
