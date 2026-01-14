#ifndef LOGGERMACROS_H
#define LOGGERMACROS_H

#include <Core/Logger/Logger.h>
#include <QString>
#include <string>

inline std::string toStdString(const QString& s)
{
    return s.toStdString();
}

inline std::string toStdString(const char* s)
{
    return std::string(s);
}

inline std::string toStdString(const std::string& s)
{
    return s;
}

// --- macros ---
#define LOG_INFO(message) \
    Logger::getInstance().log( \
        LogLevel::Info, \
        toStdString(message), \
        __FILE__, __LINE__, __func__)

#define LOG_WARNING(message) \
    Logger::getInstance().log( \
        LogLevel::Warning, \
        toStdString(message), \
        __FILE__, __LINE__, __func__)

#define LOG_ERROR(message) \
    Logger::getInstance().log( \
        LogLevel::Error, \
        toStdString(message), \
        __FILE__, __LINE__, __func__)

#endif
