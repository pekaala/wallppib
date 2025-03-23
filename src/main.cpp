#include <gtkmm.h>
#include "./include/Appbar.h"
#include "./include/Windowsize.h"
#include "./include/Log.h"

// linux:  g++ -std=c++17 main.cpp wallpaperengine.cpp -o localbuild/wallppib `pkg-config --cflags --libs gtkmm-3.0`

int main(int argc, char *argv[])
{
    Log("application start");

    windowsize winSize;

    auto app = Gtk::Application::create(argc, argv, winSize.appName);

    AppBar appbar(winSize);

    return app->run(appbar);
}
