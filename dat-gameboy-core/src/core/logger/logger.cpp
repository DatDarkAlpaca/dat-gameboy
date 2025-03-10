#include "pch.hpp"
#include "logger.hpp"

#pragma warning(push, 0)
	#include <spdlog/sinks/stdout_color_sinks.h>
	#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace dat
{
	void initialize_logger()
	{
		std::vector<spdlog::sink_ptr> sinks;

		std::string logFilename = DAT_PROJECT_NAME;
		logFilename.append(".log");

		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilename, true));

		sinks[0]->set_pattern("[%T] [%n] [%^%l%$]: %v");
		sinks[1]->set_pattern("[%D | %T] [%n] [%l]: %v");

		// Core Logger:
		MainLogger = dat::make_shared<spdlog::logger>(DAT_PROJECT_NAME, std::begin(sinks), std::end(sinks));
		spdlog::register_logger(MainLogger);

		MainLogger->set_level(spdlog::level::trace);
		MainLogger->flush_on(spdlog::level::trace);

		// Info Logger:
		InfoLogger = dat::make_shared<spdlog::logger>("Info", sinks[0]);
		spdlog::register_logger(InfoLogger);

		InfoLogger->set_level(spdlog::level::trace);
		InfoLogger->flush_on(spdlog::level::trace);
	}
}