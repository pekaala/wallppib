#ifndef WALLPAPER_MANAGER_H
#define WALLPAPER_MANAGER_H

#include <string>
#include <cstdlib>
#include <thread>
#include <X11/Xlib.h>
#include <chrono>

#include "../utils/log.h"
#include "../utils/file_type.h"

class WallpaperManager
{
public:
    void set_wallpaper(const std::string &imagePath);

private:
    void set_wallpaper_linux(const std::string &imagePath);
    void set_wallpaper_windows(const std::string &imagePath);
    void set_wallpaper_macos(const std::string &imagePath);
    bool is_gif(const std::string &imagePath);
    std::string getRootWindowID();
    int get_screen_geometry_value(int &width, int &height);
};

#endif // WALLPAPER_MANAGER_H
