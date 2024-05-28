#pragma once

#include "core/engine.h"
#include "platform/platform_detection.h"

#ifdef P_WINDOWS
int main(int argc, char** argv)
{
	prime::Game* game = prime::CreateGame();
	prime::Engine::Run(game);

	delete game;
	return 0;
}
#endif // P_WINDOWS
