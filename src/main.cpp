#include <gtkmm.h>
#include "./include/views/AppbarView.h"
#include "./include/Windowsize.h"
#include "./include/Log.h"
#include "./include/localization/LocalizationManager.h"

// linux:  g++ -std=c++17 main.cpp wallpaperengine.cpp -o localbuild/wallppib `pkg-config --cflags --libs gtkmm-3.0`

// linux vcpkg:  g++ -std=c++17 main.cpp wallpaperengine.cpp -o localbuild/wallppib -I/home/user/vcpkg/installed/x64-linux/include `pkg-config --cflags --libs gtkmm-3.0`

int main(int argc, char *argv[])
{
    try
    {
        Log("application start");

        Windowsize winSize;

        auto app = Gtk::Application::create(argc, argv, winSize.appName);

        AppbarView AppbarView(winSize);

        return app->run(AppbarView);
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}
