#ifndef LOG_H
#define LOG_H

#include <string>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

class Log
{
public:
    Log(const std::string &message);
    ~Log();

private:
    void log_error(const std::string &message);
    std::string get_current_time();
};

#endif // LOG_H
