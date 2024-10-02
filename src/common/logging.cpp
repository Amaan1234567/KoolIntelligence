#include "logging.hpp"

#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;
static inline const char *logLevelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::CRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

static inline std::string getTimestamp()
{
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

static inline std::string getThreadID()
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

static inline std::string getProcessID()
{
#ifdef _WIN32
    return std::to_string(GetCurrentProcessId());
#else
    return std::to_string(getpid());
#endif
}

void Logger::log(LogLevel level, std::string_view className, std::string_view message)
{
    if (static_cast<int>(level) >= LOG_LEVEL) {
        std::string output = "[" + getTimestamp() + "][PID:" + getProcessID() + "][TID:" + getThreadID() + "][Kool Intelligence log][" + std::string(className)
            + "][" + logLevelToString(level) + "]: " + std::string(message);

        std::clog << output << std::endl;
        if (m_logFileStream.is_open()) {
            m_logFileStream << output << std::endl;
        } else
            std::cerr << "file not open" << std::endl;
    }
}

Logger::Logger()
{
    std::cout << sizeof(LOG_FILE) << std::endl;
    if (sizeof(LOG_FILE) && !m_logFileStream.is_open()) {
        fs::path logPath(LOG_FILE);
        m_logFileStream.open(logPath, std::ios::app);
        if (!m_logFileStream.is_open()) {
            std::cerr << "Error opening log file: " << logPath.string() << std::endl;
        }
    }
}

Logger::~Logger()
{
    if (m_logFileStream.is_open()) {
        m_logFileStream.close();
    }
}