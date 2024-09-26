#pragma once

#include <fstream>
#include <string_view>

enum class LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3, CRITICAL = 4 };

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }
    void log(LogLevel level, std::string_view className, std::string_view message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    std::ofstream m_logFileStream;
};

// Convenience macros for logging
#define LOG_DEBUG(className, message) Logger::getInstance().log(LogLevel::DEBUG, className, message)
#define LOG_INFO(className, message) Logger::getInstance().log(LogLevel::INFO, className, message)
#define LOG_WARNING(className, message) Logger::getInstance().log(LogLevel::WARNING, className, message)
#define LOG_ERROR(className, message) Logger::getInstance().log(LogLevel::ERROR, className, message)
#define LOG_CRITICAL(className, message) Logger::getInstance().log(LogLevel::CRITICAL, className, message)
