#include "Logger.h"

Logger::Logger()
{
    try
    {
        std::filesystem::create_directories(Config::Log::DIRECTORY);
    }
    catch (const std::filesystem::filesystem_error &)
    {
        std::cerr << Config::Log::RED_NORMAL << "Unable to create logs directory!"
                  << Config::Log::RESET << "\n";
    }

    archiveLogFile();

    std::string path = Config::Log::DIRECTORY + "/" + Config::Log::LATEST_FILE;
    m_logFile.open(path, std::ios_base::app);

    if (!m_logFile.is_open())
    {
        std::cerr << Config::Log::RED_NORMAL << "Unable to open log file!"
                  << Config::Log::RESET << "\n";
    }
}

void Logger::archiveLogFile()
{
    std::lock_guard<std::recursive_mutex> lock(m_logMutex);
    const std::filesystem::path logsDir = Config::Log::DIRECTORY;
    const std::filesystem::path logFilePath = logsDir / Config::Log::LATEST_FILE;

    if (std::filesystem::exists(logFilePath))
    {
        int sequence = 1;
        std::filesystem::path newName;
        std::ostringstream date_oss;
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm{};
#if defined(_WIN32)
        localtime_s(&local_tm, &in_time_t);
#else
        localtime_r(&in_time_t, &local_tm);
#endif
        date_oss << std::put_time(&local_tm, "%Y-%m-%d");
        do
        {
            newName = logsDir / (Config::Log::ARCHIVE_FILE_PREFIX + date_oss.str() + "-" + std::to_string(sequence++) + ".log");
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
            if (entry.is_regular_file() &&
                entry.path().extension() == ".log" &&
                entry.path().filename() != Config::Log::LATEST_FILE)
            {
                oldLogs.push_back(entry);
            }
        }

        if (oldLogs.size() > Config::Log::MAX_ARCHIVE_FILES)
        {
            std::sort(oldLogs.begin(), oldLogs.end(), [](const auto &a, const auto &b)
                      { return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b); });

            for (size_t i = 0; i < oldLogs.size() - Config::Log::MAX_ARCHIVE_FILES; ++i)
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

Logger::~Logger()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(const Engine::LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function)
{
    std::lock_guard<std::recursive_mutex> lock(m_logMutex);
    std::string formatted = formatLogMessage(type, message, file, line, function);

    if (m_logFile.is_open())
    {
        m_logFile << formatted << std::endl;
    }

    std::string color = Engine::ConsoleColor::Reset;
    if (type == Engine::LogLevel::Error)
        color = Engine::ConsoleColor::RedBold;
    else if (type == Engine::LogLevel::Warning)
        color = Engine::ConsoleColor::YellowBold;
    else if (type == Engine::LogLevel::Info)
        color = Engine::ConsoleColor::GreenNormal;

    std::cout << color << formatted << Engine::ConsoleColor::Reset << std::endl;
}

std::string Logger::formatLogMessage(const Engine::LogLevel type, const std::string &message, const std::string &file, int line, const std::string &function)
{
    std::ostringstream oss;
    oss << "[" << logLevelToString(type) << ": " << getCurrentTime() << " | " << stripPath(file) << ":" << line << " | " << function << "] " << message;
    return oss.str();
}

std::string Logger::logLevelToString(const Engine::LogLevel type)
{
    switch (type)
    {
    case Engine::LogLevel::Info:
        return "Info";
    case Engine::LogLevel::Error:
        return "Error";
    case Engine::LogLevel::Warning:
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

std::string Logger::stripPath(const std::string &path)
{
    size_t pos = path.find_last_of("\\/");
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}
