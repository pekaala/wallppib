#ifndef DIRECTORYPATH_H
#define DIRECTORYPATH_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class DirectoryPath
{

public:
    std::string getDataContents()
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

    std::string getDataIcon()
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

    /* std::string getDataContents()
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

    std::string getDataIcon()
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();
        }

        fs::path iconsDirectory = currentDirectory / "src" / "data" / "icons";

        std::string dataStr = iconsDirectory.string();
        return dataStr;
    }
    */
};

#endif // DIRECTORYPATH_H
