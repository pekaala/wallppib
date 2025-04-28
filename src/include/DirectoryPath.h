#ifndef DIRECTORY_PATH_H
#define DIRECTORY_PATH_H

#include <filesystem>
#include <string>

using namespace std;
namespace fs = filesystem;

class DirectoryPath
{

public:
    static string GetDataContentsPath()
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            fs::path currentDirectory = fs::absolute(fs::current_path());

            if (currentDirectory.filename() == "build")
            {
                currentDirectory = currentDirectory.parent_path();
            }

            fs::path dataDirectory = currentDirectory / "src" / "data";

            string dataStr = dataDirectory.string();
            return dataStr;
        }
        else // for localBuild
        {
            fs::path currentDirectoryLocal = fs::current_path();

            fs::path srcDirectory = currentDirectoryLocal.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "data";

            string dataStr = dataDirectory.string();
            return dataStr;
        }
    }

    static string GetDataIconPath()
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();

            fs::path iconsDirectory = currentDirectory / "src" / "data" / "icons";

            string dataStr = iconsDirectory.string();
            return dataStr;
        }
        else // for localBuild
        {
            fs::path currentDirectoryLocal = fs::current_path();

            fs::path srcDirectory = currentDirectoryLocal.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "data";
            fs::path iconsDirectory = dataDirectory / "icons";

            string dataStr = iconsDirectory.string();
            return dataStr;
        }
    }

    static string GetDataLocalizationPath()
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();

            fs::path iconsDirectory = currentDirectory / "src" / "data" / "locales";

            string dataStr = iconsDirectory.string();
            return dataStr;
        }
        else // for localBuild
        {
            fs::path currentDirectoryLocal = fs::current_path();

            fs::path srcDirectory = currentDirectoryLocal.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "data";
            fs::path iconsDirectory = dataDirectory / "locales";

            string dataStr = iconsDirectory.string();
            return dataStr;
        }
    }

    static void OpenContainingFolder(const string &file_path)
    {
        string folder_path = get_folder_path(file_path);

#ifdef _WIN32
        string command = "explorer \"" + folder_path + "\"";
#elif __APPLE__
        string command = "open \"" + folder_path + "\"";
#else
        string command = "xdg-open \"" + folder_path + "\"";
#endif

        system(command.c_str());
    }

private:
    static string get_folder_path(const string &file_path)
    {
        size_t found = file_path.find_last_of("/\\");
        return file_path.substr(0, found);
    }
};

#endif // DIRECTORY_PATH_H
