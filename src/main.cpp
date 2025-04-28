#include <gtkmm.h>
#include "./include/views/AppbarView.h"
#include "./include/common/WindowSize.h"
#include "./include/Log.h"
#include "./include/localization/LocalizationManager.h"
#include "./include/DirectoryPath.h"

// linux vcpkg: g++ -std=c++17 main.cpp wallpapermanager.cpp -o localbuild/wallppib -I/home/user/vcpkg/installed/x64-linux/include `pkg-config --cflags --libs gtkmm-3.0 opencv4 Magick++ ` -lX11

int main(int argc, char *argv[])
{
    try
    {
        Log("application start");

        WindowSize winSize;

        auto app = Gtk::Application::create(argc, argv, winSize.appName);

        AppbarView AppbarView(winSize);

        // Gtk::Main::run();

        return app->run(AppbarView);
    }
    catch (const std::exception &e)
    {
        Log(e.what());
    }
}