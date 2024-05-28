
#include "pch.h"
#include "engine.h"
#include "logger.h"

namespace prime {

	void Engine::Run()
	{
		//init
		Logger::Init();
		P_INFO("welcome to prime engine");

		// update

		// shutdown
		Logger::Shutdown();
	}
}