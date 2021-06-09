#pragma once

#include "Logger.h"



#define LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(Tempest::Logger::Get(), __VA_ARGS__)
#define LOG_INFO(...)		SPDLOG_LOGGER_INFO(Tempest::Logger::Get(), __VA_ARGS__)
#define LOG_WARN(...)		SPDLOG_LOGGER_WARN(Tempest::Logger::Get(), __VA_ARGS__)
#define LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(Tempest::Logger::Get(), __VA_ARGS__)
#define LOG_CRITICAL(...)	do {SPDLOG_LOGGER_CRITICAL(Tempest::Logger::Get(), __VA_ARGS__); __debugbreak(); } while(false)
#define LOG_ASSERT_V(x, ...)	do {if(!(x)) { LOG_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); } } while(false)
#define LOG_ASSERT(x)	do {if(!(x)) { LOG_CRITICAL("Assertion Failed!"); } } while(false)

#define LOG(...)			LOG_INFO(__VA_ARGS__)