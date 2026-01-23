#ifndef LOGGER_H
#define LOGGER_H

#include <Core/Config/Configuration.h>
#include <Core/Config/LoggerMacros.h>
#include <Core/Common/GameTypes.h>
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
public:
    static Logger &getInstance();

    void log(Engine::LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function);

private:
    Logger();

    ~Logger();

    std::string formatLogMessage(Engine::LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function);

    void archiveLogFile();

    static std::string logLevelToString(Engine::LogLevel type);

    static std::string getCurrentTime();

    static std::string stripPath(const std::string &path);

    std::ofstream m_logFile;

    std::recursive_mutex m_logMutex;
};

#endif
