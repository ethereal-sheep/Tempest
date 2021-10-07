/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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