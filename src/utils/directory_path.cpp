#include "../../include/utils/directory_path.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <array>

/*PUBLIC*/
void DirectoryPath::initialize_user_config()
{
    const char *home = std::getenv("HOME");
    if (!home)
        return;

    fs::path userDataDir = fs::path(home) / ".local" / "share" / "wallppib";
    fs::path userLocalesDir = userDataDir / "locales";
    fs::path userLocaleLog = userDataDir / "logs";
    fs::path userIconsDir = userDataDir / "icons";
    fs::path userDataDirData = userDataDir / "data";
    fs::path userApplicationsDir = fs::path(home) / ".local" / "share" / "wallppib" / "applications";

    if (!fs::exists(userLocalesDir))
        fs::create_directories(userLocalesDir);

    if (!fs::exists(userLocaleLog))
        fs::create_directories(userLocaleLog);

    if (!fs::exists(userIconsDir))
        fs::create_directories(userIconsDir);

    if (!fs::exists(userDataDirData))
        fs::create_directories(userDataDirData);

    if (!fs::exists(userApplicationsDir))
        fs::create_directories(userApplicationsDir);

    char exePath[1024];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len == -1)
        return;

    exePath[len] = '\0';
    fs::path exeDir = fs::path(std::string(exePath)).parent_path();

    fs::path bundledLocalesDir = exeDir / ".." / "share" / "wallppib" / "locales";
    fs::path bundledStyle = exeDir / ".." / "share" / "wallppib" / "style.css";
    fs::path bundledLog = exeDir / ".." / "share" / "wallppib" / "errors.log";
    fs::path bundledIconsDir = exeDir / ".." / "share" / "wallppib" / "icons";
    fs::path bundledDataDir = exeDir / ".." / "share" / "wallppib" / "data";
    fs::path bundledDesktop = exeDir / ".." / "share" / "applications" / "wallppib.desktop";

    if (fs::exists(bundledLocalesDir))
    {
        for (auto &file : fs::directory_iterator(bundledLocalesDir))
        {
            fs::copy(file.path(), userLocalesDir / file.path().filename(), fs::copy_options::overwrite_existing);
        }
    }

    if (fs::exists(bundledStyle))
    {
        fs::copy(bundledStyle, userDataDir / "style.css", fs::copy_options::overwrite_existing);
    }

    if (fs::exists(bundledLog))
    {
        fs::copy(bundledLog, userLocaleLog / "errors.log", fs::copy_options::overwrite_existing);
    }

    if (fs::exists(bundledIconsDir))
    {
        for (auto &file : fs::directory_iterator(bundledIconsDir))
        {
            fs::copy(file.path(), userIconsDir / file.path().filename(), fs::copy_options::overwrite_existing);
        }
    }

    if (fs::exists(bundledDataDir))
    {
        for (auto &file : fs::directory_iterator(bundledDataDir))
        {
            fs::copy(file.path(), userDataDirData / file.path().filename(), fs::copy_options::overwrite_existing);
        }
    }

    if (fs::exists(bundledDesktop))
    {
        fs::copy(bundledDesktop, userApplicationsDir / "wallppib.desktop", fs::copy_options::overwrite_existing);
    }
}

void DirectoryPath::create_desktop_entry_if_missing(const string &path)
{
    const char *home = std::getenv("HOME");
    if (!home)
        return;

    std::string desktopDir = get_desktop_dir();
    if (desktopDir.empty())
        return;

    std::filesystem::path homePath(home);
    std::filesystem::path desktopFile = std::filesystem::path(desktopDir) / "wallppib.desktop";
    std::filesystem::path applicationsFile = homePath / ".local/share/applications/wallppib.desktop";
    std::filesystem::path iconSource = std::filesystem::path(path).parent_path() / "usr" / "share" / "wallppib" / "icons" / "logo_icon.svg";
    std::filesystem::path iconTarget = homePath / ".local" / "share" / "wallppib" / "icons" / "logo_icon.svg";

    if (std::filesystem::exists(desktopFile))
        return;

    std::filesystem::create_directories(iconTarget.parent_path());
    std::filesystem::create_directories(applicationsFile.parent_path());

    if (std::filesystem::exists(iconSource))
    {
        std::filesystem::copy_file(iconSource, iconTarget, std::filesystem::copy_options::overwrite_existing);
    }

    std::ofstream out(desktopFile);
    if (!out)
        return;

    out << "[Desktop Entry]\n";
    out << "Name=Wallppib\n";
    out << "Comment=Wallpaper Management Application\n";
    out << "Exec=" << path << "\n";
    out << "Icon=" << iconTarget.string() << "\n";
    out << "Type=Application\n";
    out << "Categories=Utility;Graphics;\n";
    out << "StartupNotify=true\n";
    out.close();

    std::filesystem::permissions(desktopFile,
                                 std::filesystem::perms::owner_all |
                                     std::filesystem::perms::group_read | std::filesystem::perms::group_exec |
                                     std::filesystem::perms::others_read | std::filesystem::perms::others_exec,
                                 std::filesystem::perm_options::replace);

    std::filesystem::copy_file(desktopFile, applicationsFile, std::filesystem::copy_options::overwrite_existing);

    std::string gio_cmd = "gio set \"" + desktopFile.string() + "\" metadata::trusted yes";
    std::system(gio_cmd.c_str());
}

string DirectoryPath::get_data_contents_path()
{
    const char *dataHome = std::getenv("HOME");
    if (dataHome)
    {
        fs::path stylePath;

        if (dataHome)
            stylePath = fs::path(dataHome) / ".local" / "share" / "wallppib" / "data";
        else
            stylePath = "/tmp/wallppib/data";

        if (fs::exists(stylePath))
        {
            return fs::canonical(stylePath).string();
        }
    }
    else
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
    return "";
}

string DirectoryPath::get_data_icon_path()
{
    char exePath[1024];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len != -1)
    {
        exePath[len] = '\0';
        fs::path exeDir = fs::path(std::string(exePath)).parent_path();

        fs::path appImageDataPath = exeDir / ".." / "share" / "wallppib" / "icons";

        if (fs::exists(appImageDataPath))
        {
            return fs::canonical(appImageDataPath).string();
        }
    }

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

string DirectoryPath::get_data_localization_path()
{
    const char *localizationHome = std::getenv("HOME");
    if (localizationHome)
    {
        fs::path stylePath;

        if (localizationHome)
            stylePath = fs::path(localizationHome) / ".local" / "share" / "wallppib" / "locales";
        else
            stylePath = "/tmp/wallppib/locales";

        if (fs::exists(stylePath))
        {
            return fs::canonical(stylePath).string();
        }
    }
    else
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
    return "";
}

string DirectoryPath::get_style_path()
{
    const char *styleHome = std::getenv("HOME");
    if (styleHome)
    {
        fs::path stylePath;

        if (styleHome)
            stylePath = fs::path(styleHome) / ".local" / "share" / "wallppib" / "style.css";
        else
            stylePath = "/tmp/wallppib/style.css";

        if (fs::exists(stylePath))
        {
            return fs::canonical(stylePath).string();
        }
    }
    else
    {
        fs::path currentDirectory = fs::absolute(fs::current_path());

        if (currentDirectory.filename() == "build")
        {
            currentDirectory = currentDirectory.parent_path();
            fs::path stylePath = currentDirectory / "src" / "assets" / "style.css";
            return stylePath.string();
        }
        else // for local build
        {
            fs::path srcDirectory = currentDirectory.parent_path().parent_path() / "src";
            fs::path stylePath = srcDirectory / "assets" / "style.css";
            return stylePath.string();
        }
    }
    return "";
}

void DirectoryPath::open_containing_folder(const string &file_path)
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

/*PRIVATE*/

string DirectoryPath::get_folder_path(const string &file_path)
{
    size_t found = file_path.find_last_of("/\\");
    return file_path.substr(0, found);
}

string DirectoryPath::get_desktop_dir()
{
    std::array<char, 128> buffer;
    std::string result;
    FILE *pipe = popen("xdg-user-dir DESKTOP", "r");
    if (!pipe)
        return "";
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }
    pclose(pipe);
    if (!result.empty() && result.back() == '\n')
    {
        result.pop_back();
    }
    return result;
}
