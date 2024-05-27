
#include "pch.h"
#include "engine.h"
#include "logger.h"
#include "prime/renderer/renderer.h"

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
		Renderer::s_API = gameConfig.API;
		Renderer::Init(s_platform.GetWindowHandle());
		Renderer::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});

		game->Init();
		s_running = true;

		// update
		while (s_running)
		{
			s_platform.Update();
			Renderer::Clear();

			game->Update();
			Renderer::SwapBuffers();
		}

		// shutdown
		game->Shutdown();
		Renderer::Shutdown();
		s_platform.Shutdown();
		Logger::Shutdown();
	}
}