
#include "pch.h"
#include "engine.h"
#include "logger.h"

namespace prime {

	static b8 s_running = false;

	void Engine::Run(Game* game)
	{
		//init
		Logger::Init();
		P_INFO("welcome to prime engine");

		game->Init();
		s_running = true;

		// update
		while (s_running)
		{
			game->Update();
		}

		// shutdown
		game->Shutdown();
		Logger::Shutdown();
	}
}