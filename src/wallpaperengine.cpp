#include "./include/WallpaperEngine.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include "./include/Log.h"
#include <thread>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void WallpaperEngine::SetWallpaper(const std::string &imagePath)
{
    // set_wallpaper_linux(imagePath);

#ifdef _WIN32
    set_wallpaper_windows(imagePath);
#elif __linux__
    set_wallpaper_linux(imagePath);
#endif

    /*#if defined(_WIN32) || defined(_WIN64)
        set_wallpaper_windows(imagePath);
    #elif defined(__linux__)*/
    /*#elif defined(__APPLE__) || defined(__MACH__)
        set_wallpaper_macos(imagePath);
    #elif defined(__unix__)
        std::cout << "unix" << std::endl;
    #else
        std::cout << "unknow? os" << std::endl;
    #endif*/
}

void WallpaperEngine::set_wallpaper_linux(const std::string &imagePath)
{
    try
    {
        const char *desktopEnv = std::getenv("XDG_CURRENT_DESKTOP");
        const char *desktopSession = std::getenv("DESKTOP_SESSION");

        if (desktopEnv == nullptr && desktopSession == nullptr)
        {
            Log("Desktop environment not detected");

            std::cerr << "Desktop environment not detected!" << std::endl;
            return;
        }

        std::string desktop = desktopEnv ? desktopEnv : desktopSession;

        std::cout << "Detected desktop environment: " << desktop << std::endl;

        if (desktop.find("GNOME") != std::string::npos)
        {
            Log("GNOME");

            /*if (is_gif(imagePath))
            {
                Log("is_gif" + imagePath);

                std::string WID = getRootWindowID();
                if (WID.empty())
                {

                    std::cerr << "Hata: WID alınamadı!" << std::endl;
                    return;
                }

                std::string command = "xwinwrap -g 1920x1080  -ni -s -nf -- gifview -w " + WID + " " + imagePath + " -a";

                system(command.c_str());

                      std::string command = "bash -c \"xwinwrap -ni -o 1.0 -fs -s -st -sp -b -nf -- "
                                           "mpv --wid=$(xwininfo -root | awk '/Window id/{print $4}') "
                                           "--loop=inf --no-border --no-osc --no-input-default-bindings "
                                           "--no-stop-screensaver --panscan=1.0 --mute '" +
                                           imagePath + "'\" &";

                     Log("Executing: " + command);

                     std::thread([command]()
                                 { system(command.c_str()); })
                         .detach();

                return;
            }*/

            std::string command = "gsettings set org.gnome.desktop.background picture-uri-dark '" + imagePath + "'";
            int result = system(command.c_str());
            if (result != 0)
            {
                std::cerr << "Error setting wallpaper with gsettings: " << result << std::endl;
            }
        }
        /*         else if (desktop.find("KDE") != std::string::npos)
                {
                    Log("KDE");

                    std::string command = "qdbus org.kde.plasmashell /PlasmaShell org.kde.PlasmaShell.evaluateScript \"workspace.wallpaperPlugin = 'org.kde.image'; workspace.wallpaperData = 'file://" + imagePath + "'\"";
                    int result = system(command.c_str());
                    if (result != 0)
                    {
                        std::cerr << "Error setting wallpaper with qdbus: " << result << std::endl;
                    }
                }
                else if (desktop.find("XFCE") != std::string::npos)
                {
                    Log("XFCE");

                    std::string command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s '" + imagePath + "'";
                    int result = system(command.c_str());
                    if (result != 0)
                    {
                        std::cerr << "Error setting wallpaper with xfconf-query: " << result << std::endl;
                    }
                }
                else if (desktop.find("MATE") != std::string::npos)
                {
                    Log("MATE");

                    std::string command = "dconf write /org/mate/desktop/background/picture-filename \"'file://" + imagePath + "'\"";
                    int result = system(command.c_str());
                    if (result != 0)
                    {
                        std::cerr << "Error setting wallpaper with dconf: " << result << std::endl;
                    }
                }
                else
                {
                    Log("Unsupported desktop environment");

                    std::cerr << "Unsupported desktop environment: " << desktop << std::endl;
                } */
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}

void WallpaperEngine::set_wallpaper_windows(const std::string &imagePath)
{
    try
    {
#if defined(_WIN32) || defined(_WIN64)
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void *)imagePath.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
#endif
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}

void WallpaperEngine::set_wallpaper_macos(const std::string &imagePath)
{
    try
    {
        /* code */
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}

bool WallpaperEngine::is_gif(const std::string &imagePath)
{
    return imagePath.size() > 4 && imagePath.substr(imagePath.size() - 4) == ".gif";
}

std::string WallpaperEngine::getRootWindowID()
{
    FILE *pipe = popen("xwininfo -root | awk '/Window id/{print $4}'", "r");
    if (!pipe)
        return "";

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        result += buffer;

    pclose(pipe);

    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}
