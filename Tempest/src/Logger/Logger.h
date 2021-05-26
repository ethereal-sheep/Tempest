#pragma once

#define SPDLOG_ACTIVE_LEVEL 0
#include "spdlog/spdlog.h"


static const char* SPDLOG_PATTERN = "[%D %T][thread:%-5t][%^%=9l%$] %v (%s)";
static const char* SPDLOG_PATTERN_NO_FILE = "[%D %T][thread:%-5t][%^%=9l%$] %v";

namespace Tempest
{
	class Logger
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& Get() { return logger; }
	
	private:
		inline static std::shared_ptr<spdlog::logger> logger;
	};
}