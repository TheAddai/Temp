
#include "pch.h"
#include "logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace prime {

	std::shared_ptr<spdlog::logger> Logger::s_handle;

	void Logger::Init()
	{
		spdlog::set_pattern("%n: %v%$");
		s_handle = spdlog::stderr_color_mt("PRIME");
		s_handle->set_level(spdlog::level::trace);
	}

	void Logger::Shutdown()
	{
		spdlog::shutdown();
	}
}