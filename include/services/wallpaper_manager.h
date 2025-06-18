

#ifndef WALLPAPER_MANAGER_H
#define WALLPAPER_MANAGER_H

#include "../utils/log.h"
#include "../utils/file_type.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <X11/Xlib.h>
#include <chrono>

using namespace std;

class WallpaperManager
{
public:
    void set_wallpaper(const string &imagePath);

private:
    void set_wallpaper_linux(const string &imagePath);
    void set_wallpaper_windows(const string &imagePath);
    void set_wallpaper_macos(const string &imagePath);
    bool is_gif(const string &imagePath);
    string getRootWindowID();
    int get_screen_geometry_value(int &width, int &height);
};

#endif // WALLPAPER_MANAGER_H