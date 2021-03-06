#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "Core.h"
#include "CoreLogger.h"

#include <algorithm>

namespace Lamp
{
	class Log
	{
	public:
		static void Initialize();

		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

#ifdef LP_CLIENT
		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
#else
		inline static Ref<CoreLogger>& GetCoreLogger() { return s_CoreLogger; }
#endif

	private:
		static Ref<spdlog::logger> s_ClientLogger;

#ifdef LP_CLIENT
		static Ref<spdlog::logger> s_CoreLogger;
#else
		static Ref<CoreLogger> s_CoreLogger;
#endif
	};

	//Client logging macros
	#define LP_TRACE(...)			::Lamp::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define LP_INFO(...)			::Lamp::Log::GetClientLogger()->info(__VA_ARGS__)
	#define LP_WARN(...)			::Lamp::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define LP_ERROR(...)			::Lamp::Log::GetClientLogger()->error(__VA_ARGS__)
	#define LP_CRITICAL(...)		::Lamp::Log::GetClientLogger()->critical(__VA_ARGS__)

	//Core logging macros
	#define LP_CORE_TRACE(...)		::Lamp::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define LP_CORE_INFO(...)		::Lamp::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define LP_CORE_WARN(...)		::Lamp::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define LP_CORE_ERROR(...)		::Lamp::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define LP_CORE_CRITICAL(...)	::Lamp::Log::GetCoreLogger()->critical(__VA_ARGS__)
}