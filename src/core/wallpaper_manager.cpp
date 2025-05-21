#include "../../include/core/wallpaper_manager.h"
#include "../../include/utils/log.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <X11/Xlib.h>
#include <chrono>

using namespace std;

void WallpaperManager::set_wallpaper(const string &imagePath)
{
    set_wallpaper_linux(imagePath);

    /* #ifdef _WIN32
        set_wallpaper_windows(imagePath);
    #elif __linux__
        set_wallpaper_linux(imagePath);
    #endif */

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

void WallpaperManager::set_wallpaper_linux(const string &imagePath)
{
    try
    {
        const char *desktopEnv = getenv("XDG_CURRENT_DESKTOP");
        const char *desktopSession = getenv("DESKTOP_SESSION");

        if (desktopEnv == nullptr && desktopSession == nullptr)
        {
            Log("Desktop environment not detected");
            return;
        }

        string desktop = desktopEnv ? desktopEnv : desktopSession;

        if (desktop.find("GNOME") != string::npos)
        {
            Log("Detected > GNOME");

            if (is_gif(imagePath))
            {
                this_thread::sleep_for(chrono::milliseconds(3));
                int width, height;
                get_screen_geometry_value(width, height);

                system("killall xwinwrap gifview");
                this_thread::sleep_for(chrono::milliseconds(5));

                string xwinwrapCommand =
                    "xwinwrap -g " + to_string(width) + "x" + to_string(height) + "+0+0 "
                                                                                  "-ni -argb -fs -s -st -sp -b -un -nf -- "
                                                                                  "gifview -w WID \"" +
                    imagePath + "\" -a &";

                system(xwinwrapCommand.c_str());
                return;
            }

            string command = "gsettings set org.gnome.desktop.background picture-uri-dark '" + imagePath + "'";
            int result = system(command.c_str());
            if (result != 0)
            {
                Log("Error setting wallpaper with gsettings");
            }
        }
        /*         else if (desktop.find("KDE") != string::npos)
                {
                    Log("KDE");

                    string command = "qdbus org.kde.plasmashell /PlasmaShell org.kde.PlasmaShell.evaluateScript \"workspace.wallpaperPlugin = 'org.kde.image'; workspace.wallpaperData = 'file://" + imagePath + "'\"";
                    int result = system(command.c_str());
                    if (result != 0)
                    {

                    }
                }
                else if (desktop.find("XFCE") != string::npos)
                {
                    Log("XFCE");

                    string command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/image-path -s '" + imagePath + "'";
                    int result = system(command.c_str());
                    if (result != 0)
                    {

                    }
                }
                else if (desktop.find("MATE") != string::npos)
                {
                    Log("MATE");

                    string command = "dconf write /org/mate/desktop/background/picture-filename \"'file://" + imagePath + "'\"";
                    int result = system(command.c_str());
                    if (result != 0)
                    {
                    }
                }
                else
                {
                    Log("Unsupported desktop environment");

                } */
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

void WallpaperManager::set_wallpaper_windows(const string &imagePath)
{
    try
    {
#if defined(_WIN32) || defined(_WIN64)
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void *)imagePath.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
#endif
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

void WallpaperManager::set_wallpaper_macos(const string &imagePath)
{
    try
    {
        /* code */
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

bool WallpaperManager::is_gif(const string &imagePath)
{
    return imagePath.size() > 4 && imagePath.substr(imagePath.size() - 4) == ".gif";
}

string WallpaperManager::getRootWindowID()
{
    FILE *pipe = popen("xwininfo -root | awk '/Window id/{print $4}'", "r");
    if (!pipe)
        return "";

    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        result += buffer;

    pclose(pipe);

    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

int WallpaperManager::get_screen_geometry_value(int &width, int &height)
{
    Display *display = XOpenDisplay(nullptr);
    if (!display)
        return -1;

    Screen *screen = DefaultScreenOfDisplay(display);
    width = screen->width;
    height = screen->height;

    XCloseDisplay(display);
    return 0;
}