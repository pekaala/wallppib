#ifndef DIRECTORY_PATH_H
#define DIRECTORY_PATH_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class DirectoryPath
{

public:
    static std::string GetDataContentsPath()
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

            std::string dataStr = dataDirectory.string();
            return dataStr;
        }
        else // for localBuild
        {
            fs::path currentDirectoryLocal = fs::current_path();

            fs::path srcDirectory = currentDirectoryLocal.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "data";

            std::string dataStr = dataDirectory.string();
            return dataStr;
        }
    }

    static std::string GetDataIconPath()
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();

            fs::path iconsDirectory = currentDirectory / "src" / "data" / "icons";

            std::string dataStr = iconsDirectory.string();
            return dataStr;
        }
        else // for localBuild
        {
            fs::path currentDirectoryLocal = fs::current_path();

            fs::path srcDirectory = currentDirectoryLocal.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "data";
            fs::path iconsDirectory = dataDirectory / "icons";

            std::string dataStr = iconsDirectory.string();
            return dataStr;
        }
    }

    static std::string GetDataLocalizationPath()
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();

            fs::path iconsDirectory = currentDirectory / "src" / "data" / "locales";

            std::string dataStr = iconsDirectory.string();
            return dataStr;
        }
        else // for localBuild
        {
            fs::path currentDirectoryLocal = fs::current_path();

            fs::path srcDirectory = currentDirectoryLocal.parent_path().parent_path() / "src";

            fs::path dataDirectory = srcDirectory / "data";
            fs::path iconsDirectory = dataDirectory / "locales";

            std::string dataStr = iconsDirectory.string();
            return dataStr;
        }
    }
};

#endif // DIRECTORY_PATH_H
