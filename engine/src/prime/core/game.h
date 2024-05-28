#pragma once

#include "defines.h"
#include "prime/renderer/renderer.h"

#include <string>

namespace prime {

	class Game
	{
	public:
		struct Config
		{
			ui32 width = 640, height = 480;
			bool fullScreen = false, vSync = true;
			std::string title = "Prime Game";
			Renderer::API API = Renderer::API::openGL;
			Renderer::Config rendererConfig;
		};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void Update() = 0;
		virtual void ImGuiRender() = 0;

		virtual Config GetConfig() const = 0;
	};

	Game* CreateGame();
}