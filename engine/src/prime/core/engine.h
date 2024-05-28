#pragma once

#include "game.h"
#include "prime/platform/platform.h"

namespace prime {

	class Engine
	{
	public:
		static void Run(Game* game);

		static const void* GetWindowHandle() { return s_platform.GetWindowHandle(); }
		static const ui32 GetWindowWidth() { return s_platform.GetWidth(); }
		static const ui32 GetWindowHeight() { return s_platform.GetHeight(); }

	private:
		static Platform s_platform;
	};
}
