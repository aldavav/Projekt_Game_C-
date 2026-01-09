#include "Logger.h"

Logger::Logger()
{
    try
    {
        std::filesystem::create_directories("logs");
    }
    catch (const std::filesystem::filesystem_error &)
    {
        std::cerr << RED_NORMAL_TEXT << "Unable to create logs directory!" << RESET_TEXT << "\n";
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

Logger &Logger::getInstance()
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
    std::string command = "notepad.exe \"" + logFilePath + "\"";
    int rc = std::system(command.c_str());
    if (rc != 0)
    {
        std::cerr << RED_NORMAL_TEXT << "Failed to open log with notepad (rc=" << rc << ")." << RESET_TEXT << "\n";
    }
#elif defined(__APPLE__)
    std::string command = "open -a TextEdit \"" + logFilePath + "\"";
    int rc = std::system(command.c_str());
    if (rc != 0)
    {
        std::string fallback = "open \"" + logFilePath + "\"";
        std::system(fallback.c_str());
    }
#else
    std::string command = "xdg-open \"" + logFilePath + "\"";
    int rc = std::system(command.c_str());
    if (rc != 0)
    {
        std::cerr << RED_NORMAL_TEXT << "Failed to open log with xdg-open (rc=" << rc << ")." << RESET_TEXT << "\n";
        std::cerr << RED_NORMAL_TEXT << "You can open the log manually: " << logFilePath << RESET_TEXT << "\n";
    }
#endif
}

void Logger::log(const LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function)
{
    std::lock_guard<std::recursive_mutex> lock(logMutex_);
    std::string formatted = formatLogMessage(type, message, file, line, function);

    if (logFile_.is_open())
    {
        logFile_ << formatted << std::endl;
    }

    std::string color = RESET_TEXT;
    if (type == LogLevel::Error)
        color = RED_BOLD_TEXT;
    else if (type == LogLevel::Warning)
        color = YELLOW_BOLD_TEXT;
    else if (type == LogLevel::Info)
        color = GREEN_NORMAL_TEXT;

    std::cout << color << formatted << RESET_TEXT << std::endl;
}

std::string Logger::formatLogMessage(const LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function)
{
    std::ostringstream oss;
    oss << "[" << logLevelToString(type) << ": " << getCurrentTime() << " | " << stripPath(file) << ":" << line << " | " << function << "] " << message;
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
    const std::filesystem::path logsDir = "logs";
    const std::filesystem::path logFilePath = logsDir / "latest.log";

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

        int sequence = 1;
        std::filesystem::path newName;
        do
        {
            newName = logsDir / ("log-" + date_oss.str() + "-" + std::to_string(sequence++) + ".log");
        } while (std::filesystem::exists(newName));

        try
        {
            std::filesystem::rename(logFilePath, newName);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Rename failed: " << e.what() << "\n";
        }
    }

    try
    {
        std::vector<std::filesystem::directory_entry> oldLogs;

        for (const auto &entry : std::filesystem::directory_iterator(logsDir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".log" && entry.path().filename() != "latest.log")
            {
                oldLogs.push_back(entry);
            }
        }

        if (oldLogs.size() > 30)
        {
            std::sort(oldLogs.begin(), oldLogs.end(), [](const auto &a, const auto &b)
                      { return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b); });

            for (size_t i = 0; i < oldLogs.size() - 30; ++i)
            {
                std::filesystem::remove(oldLogs[i]);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Cleanup failed: " << e.what() << "\n";
    }
}

std::string Logger::stripPath(const std::string& path) {
    size_t pos = path.find_last_of("\\/");
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}
