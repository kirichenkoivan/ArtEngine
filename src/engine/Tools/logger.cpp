#include "../include/engine/Tools/logger.h"

void Logger::Log(const std::string &module, const std::string &level, const std::string &message)
{
    std::cout << GetCurrentTime() << " [" << module << "] " << " [" << level << "] " << message << std::endl;
}

void Logger::Info(const std::string &module, const std::string &message)
{
    Log(module, "Info", message);
}

void Logger::Warning(const std::string &module, const std::string &message)
{
    Log(module, "Warning", message);
}

void Logger::Error(const std::string &module, const std::string &message)
{
    Log(module, "Error", message);
}

std::string Logger::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    const char *ctime_str = std::ctime(&now_time);
    return ctime_str;
}