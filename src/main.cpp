#include <gtkmm.h>
#include "../include/ui/app_bar_window.h"
#include "../include/ui/indicators/app_indicator_impression.h"
#include "../include/common/window_size.h"
#include "../include/utils/log.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <iostream>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/notebook.h>

/*linux vcpkg:

g++ -std=c++17 main.cpp core/wallpaper_manager.cpp utils/log.cpp  utils/localization/localization_manager.cpp  utils/settings.cpp  utils/directory_path.cpp ui/alerts/message.cpp ui/indicators/app_indicator_impression.cpp ui/about_window.cpp ui/app_bar_window.cpp ui/home_window.cpp ui/settings_window.cpp -o localbuild/wallppib -I/home/user/vcpkg/installed/x64-linux/include `pkg-config --cflags --libs gtkmm-3.0 opencv4 Magick++ ayatana-appindicator3-0.1` -lX11

*/

bool already_running()
{
#ifdef _WIN32
    HANDLE hMutex = CreateMutex(nullptr, TRUE, L"MyUniqueAppMutexName");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hMutex);
        return true;
    }
    return false;
#else
    int fd = open("/tmp/myapp.lock", O_CREAT | O_RDWR, 0666);
    if (fd < 0)
        return true;

    if (flock(fd, LOCK_EX | LOCK_NB) < 0)
    {
        close(fd);
        return true;
    }

    return false;
#endif
}

int main(int argc, char *argv[])
{
    try
    {
        Log("application start");

        if (already_running())
        {
            Log("Another instance already running. Exiting");
            return 0;
        }

        WindowSize winSize;

        auto app = Gtk::Application::create(argc, argv, winSize.appName);

        Gtk::Window window;
        window.set_default_size(800, 600);
        window.set_title("Uygulama Başlığı");
        window.set_icon_name("/home/ibrahim/OpenSource/wallppib/src/data/icons/ICdelete.png");
        /*try
        {
            window.set_default_icon_from_file("/home/ibrahim/OpenSource/wallppib/src/data/icons/ICdelete.png");
            window.set_icon_from_file("/home/ibrahim/OpenSource/wallppib/src/data/icons/ICdelete.png");
        }
        catch (const Glib::FileError &e)
        {
            Log("error");
        }*/

        return app->run(window);

        /*AppBarWindow AppBarWindow(winSize, app);
        return app->run(AppBarWindow);*/
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}
