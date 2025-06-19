#ifndef HOME_WINDOW_H
#define HOME_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <atomic>

#include "../utils/localization/i_localization.h"
#include "../utils/localization/localization_manager.h"
#include "../utils/log.h"
#include "../utils/directory_path.h"
#include "../utils/file_type.h"
#include "../utils/system_info.h"
#include "../services/image_manager.h"
#include "../services/wallpaper_manager.h"
#include "../common/window_size.h"
#include "../ui/alerts/info_bar_message.h"

class HomeWindow : public Gtk::Window, public ILocalization
{
public:
    HomeWindow(LocalizationManager &locManager);
    ~HomeWindow();

    Gtk::Box &get_home_window_box();

private:
    Gtk::Box main_Box;
    Gtk::Dialog *dialog = nullptr;
    Gtk::FileChooserDialog *file_chooser_dialog = nullptr;
    Gtk::ScrolledWindow scrolled_window;
    Gtk::FlowBox flowbox;
    Gtk::ProgressBar *m_ProgressBar = nullptr;
    Gtk::Image loading_image;
    Gtk::Frame image_wrapper;
    Gtk::ComboBoxText combo_Box;
    Gtk::Label x11_error_label;
    sigc::connection m_connection_timeout;

    std::string selected_Image_Path;
    std::atomic<bool> add_result = false;
    std::atomic<bool> is_done = false;

    WindowSize win_Size;
    WallpaperManager wallpaper_Manager;
    LocalizationManager &localization_Manager;
    ImageManager image_Manager;

protected:
    void loading_animation_provider(bool loadtype);
    void load_images(bool loadtype);
    bool on_image_click(GdkEventButton *event, const Image &imageInfo);
    void on_select_image();
    void on_set_wallpaper();
    void on_delete_image();
    void open_containing_current_file();
    void on_close_dialog();
    void listener_update_ui() override;
    bool on_timeout();
    void on_change_filter();
    void on_preview_gif(std::string &filePath);
    void show_error_message_x11();
    void clear_info_bar();
};

#endif // HOME_WINDOW_H
