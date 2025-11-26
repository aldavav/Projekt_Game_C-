#ifndef LOGGERMACROS_H
#define LOGGERMACROS_H

#include "Logger.h"

#define LOG_INFO(message) Logger::getInstance().log(LogLevel::Info, message)
#define LOG_ERROR(message) Logger::getInstance().log(LogLevel::Error, message)
#define LOG_WARNING(message) Logger::getInstance().log(LogLevel::Warning, message)

#endif // LOGGERMACROS_H
