#pragma once

#include "prime/platform/platform_detection.h"

#include <spdlog/spdlog.h>

namespace prime {

	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

		static std::shared_ptr<spdlog::logger>& Get() { return s_handle; }

	private:
		static std::shared_ptr<spdlog::logger> s_handle;
	};

}

#ifdef P_DEBUG
#define P_TRACE(...)                 prime::Logger::Get()->trace(__VA_ARGS__)
#define P_INFO(...)                  prime::Logger::Get()->info(__VA_ARGS__)
#define P_WARN(...)                  prime::Logger::Get()->warn(__VA_ARGS__)
#define P_ERROR(...)                 prime::Logger::Get()->error(__VA_ARGS__)
#else
#define P_TRACE(...)                 void()0
#define P_INFO(...)                  void()0
#define P_WARN(...)                  void()0
#define P_ERROR(...)                 void()0
#endif // P_DEBUG
