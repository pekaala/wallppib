#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <exception>
#include <string>

using std::string;

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
