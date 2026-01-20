#ifndef LOGGER_H
#define LOGGER_H

#include <Core/Config/LoggerMacros.h>
#include <Core/Config/LoggerTypes.h>
#include <Core/Config/AnsiCodes.h>
#include <Core/Config/Config.h>
#include <filesystem>
#include <qlogging.h>
#include <algorithm>
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>

class Logger
{
private:
    Logger();

    ~Logger();

    std::ofstream logFile_;

    std::recursive_mutex logMutex_;

    std::string formatLogMessage(LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function);

    static std::string logLevelToString(LogLevel type);

    std::string getCurrentTime();

    void archiveLogFile();

    std::string stripPath(const std::string &path);

public:
    static Logger &getInstance();

    void log(LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function);

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    Logger(Logger &&) = delete;

    Logger &operator=(Logger &&) = delete;
};

#endif
