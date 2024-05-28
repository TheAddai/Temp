#pragma once

#include "prime/prime.h"

namespace prime {

	class Editor : public Game
	{
	public:
		void Init() override;
		void Shutdown() override;

		void Update() override;
		void ImGuiRender() override;

		Config GetConfig() const override
		{
			Config config;
			config.fullScreen = true;
			config.title = "Prime Editor";
			config.vSync = true;

			return config;
		}

	private:
		void Dockspace();
		void Viewport();

	private:
		Ref<Scene> m_scene;
	};
}