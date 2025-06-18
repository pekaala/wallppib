#include <gtkmm.h>
#include "../include/ui/app_bar_window.h"
#include "../include/common/window_size.h"
#include "../include/utils/log.h"
#include "../include/utils/directory_path.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <iostream>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/notebook.h>

using namespace std;

/*linux vcpkg:

g++ -std=c++17 main.cpp services/wallpaper_manager.cpp services/image_manager.cpp domain/image_info.cpp  utils/log.cpp  utils/localization/localization_manager.cpp  utils/settings.cpp utils/file_type.cpp  utils/directory_path.cpp utils/system_info.cpp ui/alerts/info_bar_message.cpp ui/indicators/app_indicator_impression.cpp ui/about_window.cpp ui/app_bar_window.cpp ui/home_window.cpp ui/settings_window.cpp -o localbuild/wallppib -I/home/user/vcpkg/installed/x64-linux/include `pkg-config --cflags --libs gtkmm-3.0 opencv4 Magick++ ayatana-appindicator3-0.1` -lX11

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

void load_css()
{
    string css_content_path = DirectoryPath::get_style_path();

    std::ifstream cssFile(css_content_path);
    if (!cssFile)
    {
        Log("Cannot open file");
        return;
    }

    std::stringstream buffer;
    buffer << cssFile.rdbuf();
    std::string cssContent = buffer.str();

    auto css = Gtk::CssProvider::create();
    try
    {
        Log("Css File Path:" + css_content_path);
        css->load_from_data(cssContent);
    }
    catch (const Glib::Error &ex)
    {
        Log("Cannot load css");
        return;
    }

    Gtk::StyleContext::add_provider_for_screen(
        Gdk::Screen::get_default(),
        css,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int main(int argc, char *argv[])
{
    try
    {
        Log("application start");

        /*if (already_running())
        {
            Log("Another instance already running. Exiting");
            return 0;
        }*/

        WindowSize winSize;

        auto app = Gtk::Application::create(argc, argv, winSize.appId);

        string appPath = argv[0];

        DirectoryPath::create_desktop_entry_if_missing(appPath);
        DirectoryPath::initialize_user_config();

        load_css();

        AppBarWindow AppBarWindow(winSize, app);
        return app->run(AppBarWindow);
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}
