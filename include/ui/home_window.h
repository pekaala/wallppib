#ifndef HOME_WINDOW_H
#define HOME_WINDOW_H

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/grid.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/button.h>
#include <gtkmm/layout.h>
#include <gtkmm/dialog.h>
#include <iostream>
#include <vector>
#include "../core/wallpaper_manager.h"
#include "../utils/localization/i_localization.h"
#include "../utils/localization/localization_manager.h"
#include "../utils/log.h"
#include "../utils/directory_path.h"
#include "../images/Image.h"
#include "../images/ImageManager.h"
#include "../common/window_size.h"
#include "../ui/alerts/mesage.h"

using namespace std;

class HomeWindow : public Gtk::Window, public ILocalization
{
public:
    HomeWindow(LocalizationManager &locManager);
    ~HomeWindow();

    Gtk::ScrolledWindow &get_layout_scroll();

private:
    Gtk::Layout m_Layout;
    string selected_Image_Path;
    Gtk::Button select_File_Button;
    Gtk::Dialog *dialog = nullptr;
    Gtk::Button close_Button;
    Gtk::ScrolledWindow scrolled_window;
    Gtk::FlowBox flowbox;
    WindowSize win_Size;
    WallpaperManager wallpaper_Manager;
    LocalizationManager &localization_Manager;
    ImageManager image_Manager;

protected:
    void load_images(bool loadtype);
    bool on_image_click(GdkEventButton *event, const Image &imageInfo);
    void on_select_image();
    void on_set_wallpaper();
    void delete_image();
    void open_containing_current_file();
    void on_close_dialog();
    void listener_update_ui() override;
};

#endif // HOME_WINDOW_H