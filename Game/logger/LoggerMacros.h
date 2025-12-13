#ifndef LOGGERMACROS_H
#define LOGGERMACROS_H

#include "Logger.h"

#define LOG_INFO(message) Logger::getInstance().log(LogLevel::Info, message, __FILE__, __LINE__, __func__)
#define LOG_ERROR(message) Logger::getInstance().log(LogLevel::Error, message, __FILE__, __LINE__, __func__)
#define LOG_WARNING(message) Logger::getInstance().log(LogLevel::Warning, message, __FILE__, __LINE__, __func__)

#endif
