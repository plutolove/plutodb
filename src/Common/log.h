#pragma once
#include "spdlog/spdlog.h"

#define LOG_INFO(fmt, args...) spdlog::info(fmt, ##args)
#define LOG_ERROR(fmt, args...) spdlog::error(fmt, ##args)
#define LOG_WARN(fmt, args...) spdlog::warn(fmt, ##args)
#define LOG_FATAL(fmt, args...) spdlog::critical(fmt, ##args)
