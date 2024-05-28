
#include "pch.h"
#include "engine.h"
#include "logger.h"
#include "prime/renderer/renderer.h"
#include "events.h"
#include "dispatcher.h"
#include "prime/imgui/imguiAPI.h"

namespace prime {

	Platform Engine::s_platform;
	static b8 s_running = false;

	static ImGuiAPI* s_imguiAPI = nullptr;

	static void OnWindowClose(const WindowCloseEvent&)
	{
		s_running = false;
	}

	static void OnWindowResize(const WindowResizeEvent& e)
	{
		if (e.GetWidth() > 0 && e.GetHeight() > 0)
		{
			Renderer::SetViewport(e.GetWidth(), e.GetHeight());
		}
	
	}

	void Engine::Run(Game* game)
	{
		//init
		Logger::Init();
		P_INFO("welcome to prime engine");

		Game::Config gameConfig = game->GetConfig();
		s_platform.Init(gameConfig.title, gameConfig.width, gameConfig.height, gameConfig.fullScreen);
		Renderer::s_API = gameConfig.API;
		Renderer::Init(s_platform.GetWindowHandle());
		s_platform.SetVSync(gameConfig.vSync);

		// imgui
		s_imguiAPI = ImGuiAPI::Create();
		s_imguiAPI->Init();

		game->Init();
		s_running = true;

		// subscribe to events
		Dispatcher::Get().sink<WindowCloseEvent>().connect<&OnWindowClose>();
		Dispatcher::Get().sink<WindowResizeEvent>().connect<&OnWindowResize>();

		// update
		while (s_running)
		{
			s_platform.Update();
			Dispatcher::Get().update();
			game->Update();

			s_imguiAPI->BeginRender();
			game->ImGuiRender();
			s_imguiAPI->EndRender();

			Renderer::SwapBuffers();
		}

		// shutdown
		game->Shutdown();
		Renderer::Shutdown();

		s_imguiAPI->Shutdown();
		delete s_imguiAPI;

		s_platform.Shutdown();
		Dispatcher::Get().clear();
		Logger::Shutdown();
	}
}