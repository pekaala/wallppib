

#ifndef WALLPAPER_ENGINE_H
#define WALLPAPER_ENGINE_H

#include <string>

class WallpaperEngine
{
public:
    void setWallpaper(const std::string &imagePath);

private:
    void set_wallpaper_linux(const std::string &imagePath);
    void set_wallpaper_windows(const std::string &imagePath);
    void set_wallpaper_macos(const std::string &imagePath);
    bool is_gif(const std::string &imagePath);
    std::string getRootWindowID();
};

#endif // WALLPAPER_ENGINE_H