
#include "pch.h"
#include "engine.h"
#include "logger.h"

namespace prime {

	Platform Engine::s_platform;
	static b8 s_running = false;

	void Engine::Run(Game* game)
	{
		//init
		Logger::Init();
		P_INFO("welcome to prime engine");

		Game::Config gameConfig = game->GetConfig();
		s_platform.Init(gameConfig.title, gameConfig.width, gameConfig.height, gameConfig.fullScreen);

		game->Init();
		s_running = true;

		// update
		while (s_running)
		{
			s_platform.PollEvents();
			game->Update();
			s_platform.SwapBuffers();
		}

		// shutdown
		game->Shutdown();
		s_platform.Shutdown();
		Logger::Shutdown();
	}
}