#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <string>
#include <ctime>
#include "Log.h"

using namespace std;
namespace fs = std::filesystem;

class Log
{
public:
    Log(const string &message)
    {
        log_error(message);
    }

private:
    void log_error(const string &message)
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();
        }

        fs::path srcDirectory = currentDirectory / "src";
        fs::path logsDirectory = srcDirectory / "logs";

        if (!fs::exists(logsDirectory))
        {
            fs::create_directories(logsDirectory);
        }

        string logFilePath = logsDirectory.string() + "/errors.log";

        ofstream logFile(logFilePath, ios::app);
        if (logFile.is_open())
        {
            logFile << get_current_time() << " - " << message << endl;
            logFile.close();
        }
        else
        {
            cerr << "Error (" << logFilePath << ")" << endl;
        }
    }

    string get_current_time()
    {
        time_t now = time(nullptr);
        tm *ltm = localtime(&now);

        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
        return string(buffer);
    }
};

#endif // LOG_H
