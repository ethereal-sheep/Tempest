/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Core.h"
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


	char* pValue;
	size_t len;
	[[maybe_unused]] errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
	if (pValue)
	{
		tpath path{ pValue };
		free(pValue); // It's OK to call free with NULL
		path /= "Documents";
		path /= "CoReSys";
		path /= "Log/Log.log";
		// should use env variable getenv("APPDATA/SomeFolder")
		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string().c_str()));
	}

	logger = spdlog::create_async<spdlog::sinks::dist_sink_mt>("Global Logger", sinks);
	logger->set_pattern(SPDLOG_PATTERN_NO_FILE);
	logger->set_level(spdlog::level::trace);
}
