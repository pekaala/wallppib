#include "../../include/utils/log.h"
#include <filesystem>
#include <string>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

Log::Log(const string &message)
{
    log_error(message);
}

Log::~Log()
{
}

void Log::log_error(const string &message)
{
    fs::path currentDirectory = fs::absolute(fs::current_path());

    if (currentDirectory.filename() == "build")
    {
        currentDirectory = currentDirectory.parent_path();
    }

    // fs::path srcDirectory = currentDirectory / "src";
    fs::path srcDirectory = currentDirectory;

    const char *snapUserCommon = std::getenv("SNAP_USER_COMMON");
    fs::path logsDirectory;

    if (snapUserCommon)
    {
        logsDirectory = fs::path(snapUserCommon) / "logs";
    }
    else
    {
        logsDirectory = srcDirectory / "logs";
    }

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

string Log::get_current_time()
{
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}
