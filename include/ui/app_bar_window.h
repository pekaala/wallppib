#ifndef APP_BAR_WINDOW_H
#define APP_BAR_WINDOW_H

#include <gtkmm.h>
#include <string>

#include "about_window.h"
#include "home_window.h"
#include "settings_window.h"
#include "../common/window_size.h"
#include "../utils/localization/i_localization.h"
#include "../utils/localization/localization_manager.h"
#include "../utils/log.h"
#include "../utils/directory_path.h"
#include "../ui/indicators/app_indicator_impression.h"

class AppBarWindow : public Gtk::Window, public ILocalization
{
public:
    AppBarWindow(WindowSize winSize, Glib::RefPtr<Gtk::Application> app_ref);
    ~AppBarWindow();

private:
    Gtk::HeaderBar header_Bar;
    Gtk::Button menu_button;
    Gtk::Menu menu;
    Gtk::MenuItem menu_About, menu_Settings, menu_Quit;
    Gtk::Box m_VBox;
    Gtk::Notebook m_Notebook;
    Glib::RefPtr<Gtk::Application> app;
    AboutWindow *about_window;
    HomeWindow *home_window;
    SettingsWindow *settings_window;
    AppIndicatorImpression *indicator;
    LocalizationManager localization_Manager;

protected:
    void apply_header_bar_menu();
    void on_about_dialog();
    void on_settings_dialog();
    bool send_background_app(GdkEventAny *event);
    void listener_update_ui() override;
};

#endif // APP_BAR_WINDOW_H
