#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <string>
#include <ctime>
#include "Log.h"

namespace fs = std::filesystem;

class Log
{
public:
    Log(const std::string &message)
    {
        log_error(message);
    }

private:
    std::string mainDirectory;

    /*void log_error(const std::string &message)
        {
            fs::path currentDirectory = fs::current_path();

            fs::path srcDirectory = currentDirectory.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "logs";

            mainDirectory = dataDirectory.string();

            std::string logFilePath = mainDirectory + "/errors.log";

            if (!fs::exists(mainDirectory))
            {
                fs::create_directory(mainDirectory);
            }

            std::ofstream logFile(logFilePath, std::ios::app);
            if (logFile.is_open())
            {
                logFile << get_current_time() << " - " << message << std::endl;
                logFile.close();
            }
            else
            {
                std::cerr << "Log file not active!" << std::endl;
            }
        } */

    void log_error(const std::string &message)
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

        std::string logFilePath = logsDirectory.string() + "/errors.log";

        std::ofstream logFile(logFilePath, std::ios::app);
        if (logFile.is_open())
        {
            logFile << message << std::endl;
            logFile.close();
        }
        else
        {
            std::cerr << "Error (" << logFilePath << ")" << std::endl;
        }
    }

    std::string get_current_time()
    {
        std::time_t now = std::time(nullptr);
        std::tm *ltm = std::localtime(&now);

        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
        return std::string(buffer);
    }
};

#endif // LOG_H
