#ifndef LOG_H
#define LOG_H

#include <filesystem>
#include <string>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

class Log
{
public:
    Log(const string &message);
    ~Log();

private:
    void log_error(const string &message);
    string get_current_time();
};

#endif // LOG_H
