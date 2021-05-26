
#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/msvc_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/dist_sink.h"
#include "spdlog/sinks/basic_file_sink.h"



void Tempest::Logger::Init()
{
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log/Log.log"));

	logger = spdlog::create_async<spdlog::sinks::dist_sink_mt>("Global Logger", sinks);
	logger->set_pattern(SPDLOG_PATTERN_NO_FILE);
	logger->set_level(spdlog::level::trace);
}
