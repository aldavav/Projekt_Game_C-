#include "Logger.h"

#include <sstream>
#include <iomanip>
#include <cstdlib>      // system()
#include <iostream>

Logger::Logger()
{
    try {
        std::filesystem::create_directories("logs");
    } catch (...) {
        // ignore filesystem errors here; will be handled when opening file
    }

    archiveLogFile();

    logFile_.open("logs/latest.log", std::ios_base::app);
    if (!logFile_.is_open())
    {
        std::cerr << RED_NORMAL_TEXT << "Unable to open log file!" << RESET_TEXT << "\n";
    }
}

Logger::~Logger()
{
    if (logFile_.is_open())
    {
        logFile_.close();
    }
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::showLog()
{
    const std::filesystem::path logPath = std::filesystem::path("logs") / "latest.log";
    const std::string logFilePath = logPath.string();

    if (!std::filesystem::exists(logPath))
    {
        std::cerr << RED_NORMAL_TEXT << "Log file not found: " << logFilePath << RESET_TEXT << "\n";
        return;
    }

#ifdef _WIN32
    // On Windows call notepad (no windows.h dependency)
    std::string command = "notepad.exe \"" + logFilePath + "\"";
    int rc = std::system(command.c_str());
    if (rc != 0) {
        std::cerr << RED_NORMAL_TEXT << "Failed to open log with notepad (rc=" << rc << ")." << RESET_TEXT << "\n";
    }
#elif defined(__APPLE__)
    // macOS: open with TextEdit
    std::string command = "open -a TextEdit \"" + logFilePath + "\"";
    int rc = std::system(command.c_str());
    if (rc != 0) {
        // fallback: open default application
        std::string fallback = "open \"" + logFilePath + "\"";
        std::system(fallback.c_str());
    }
#else
    // Linux / other: use xdg-open if available
    std::string command = "xdg-open \"" + logFilePath + "\"";
    int rc = std::system(command.c_str());
    if (rc != 0) {
        std::cerr << RED_NORMAL_TEXT << "Failed to open log with xdg-open (rc=" << rc << ")." << RESET_TEXT << "\n";
        std::cerr << RED_NORMAL_TEXT << "You can open the log manually: " << logFilePath << RESET_TEXT << "\n";
    }
#endif
}

void Logger::log(const LogLevel type, const std::string& message) {
    std::lock_guard<std::recursive_mutex> lock(logMutex_);

    if (logFile_.is_open()) {
        logFile_ << formatLogMessage(type, message) << std::endl;
    } else {
        std::cerr << RED_NORMAL_TEXT << "Unable to open log file!" << RESET_TEXT << std::endl;
    }
}

std::string Logger::formatLogMessage(const LogLevel type, const std::string& message)
{
    std::ostringstream oss;
    oss << "[" << logLevelToString(type) << ": " << getCurrentTime() << "] " << message;
    return oss.str();
}

std::string Logger::logLevelToString(const LogLevel type)
{
    switch (type)
    {
    case LogLevel::Info:
        return "Info";
    case LogLevel::Error:
        return "Error";
    case LogLevel::Warning:
        return "Warning";
    default:
        return "Unknown";
    }
}

std::string Logger::getCurrentTime()
{
    std::lock_guard<std::recursive_mutex> lock(logMutex_);

    const auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm{};
#if defined(_WIN32)
    localtime_s(&local_tm, &in_time_t);
#else
    localtime_r(&in_time_t, &local_tm);
#endif

    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setw(6) << std::setfill('0') << us.count();
    return oss.str();
}

void Logger::archiveLogFile()
{
    std::lock_guard<std::recursive_mutex> lock(logMutex_);

    const std::filesystem::path logFilePath = std::filesystem::path("logs") / "latest.log";
    if (std::filesystem::exists(logFilePath))
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm local_tm{};
#if defined(_WIN32)
        localtime_s(&local_tm, &in_time_t);
#else
        localtime_r(&in_time_t, &local_tm);
#endif

        std::ostringstream date_oss;
        date_oss << std::put_time(&local_tm, "%Y-%m-%d");
        std::string date_str = date_oss.str();

        int sequence_number = 1;
        std::filesystem::path newLogFileName;

        do
        {
            std::ostringstream new_oss;
            new_oss << "logs/log-" << date_str << "-" << sequence_number << ".log";
            newLogFileName = new_oss.str();
            sequence_number++;
        }
        while (std::filesystem::exists(newLogFileName));

        try {
            std::filesystem::rename(logFilePath, newLogFileName);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << RED_NORMAL_TEXT << "Failed to archive log file: " << e.what() << RESET_TEXT << "\n";
        }
    }
}
