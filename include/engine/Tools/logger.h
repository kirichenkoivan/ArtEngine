#ifndef LOGGER
#define LOGGER

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

class Logger
{

public:
    static Logger &GetInstance()
    {
        static Logger instance;
        return instance;
    };

    void Log(const std::string &module, const std::string &level, const std::string &message);
    void Info(const std::string &module, const std::string &message);
    void Warning(const std::string &module, const std::string &message);
    void Error(const std::string &module, const std::string &message);

private:
    Logger() {};
    std::string GetCurrentTime();
};

#endif