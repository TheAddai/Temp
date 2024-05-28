#pragma once

#include "game.h"
#include "prime/platform/platform.h"

namespace prime {

	class Engine
	{
	public:
		static void Run(Game* game);

	private:
		static Platform s_platform;
	};
}
