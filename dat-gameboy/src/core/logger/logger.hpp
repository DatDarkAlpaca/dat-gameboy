#pragma once
#include "core/defines.hpp"
#include "core/memory/memory.hpp"

#pragma warning(push, 0)
	#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace dat
{
	inline dat_shared<spdlog::logger> MainLogger;
	inline dat_shared<spdlog::logger> InfoLogger;

	void initialize_logger();
}

#ifdef DAT_DEBUG
	#define DAT_LOG_CRITICAL(...)	{ ::dat::MainLogger->critical(__VA_ARGS__); DAT_BREAKPOINT(); }
	#define DAT_LOG_ERROR(...)		  ::dat::MainLogger->error(__VA_ARGS__)
	#define DAT_LOG_WARN(...)		  ::dat::MainLogger->warn(__VA_ARGS__)
	#define DAT_LOG_TRACE(...)		  ::dat::InfoLogger->trace(__VA_ARGS__)
	#define DAT_LOG_INFO(...)		  ::dat::InfoLogger->info(__VA_ARGS__)

	#if defined(DAT_ENABLE_DEBUG_LOG)
		#define DAT_LOG_DEBUG(...)		  ::dat::MainLogger->debug(__VA_ARGS__)
	#else
		#define DAT_LOG_DEBUG(...)
	#endif
	

#else
	#define DAT_LOG_CRITICAL(...)	{ ::dat::MainLogger->critical(__VA_ARGS__); DAT_BREAKPOINT(); }
	#define DAT_LOG_ERROR(...)		  ::dat::MainLogger->error(__VA_ARGS__)
	#define DAT_LOG_WARN(...)		{ }
	#define DAT_LOG_DEBUG(...)		{ }
	#define DAT_LOG_TRACE(...)		{ }
	#define DAT_LOG_INFO(...)		{ }
#endif