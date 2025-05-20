
#ifndef APP_INDICATOR_STATUS_H
#define APP_INDICATOR_STATUS_H

#include <gtkmm.h>
#include <libayatana-appindicator/app-indicator.h>
#include <string>
#include "../../utils/directory_path.h"
#include "../../utils/localization/i_localization.h"
#include "../../utils/localization/localization_manager.h"
#include "../../common/window_size.h"

using namespace std;

class AppIndicatorImpression : public Gtk::Window, public ILocalization
{
public:
    AppIndicatorImpression(LocalizationManager &locManager, Glib::RefPtr<Gtk::Application> app_ref);
    ~AppIndicatorImpression();

private:
    AppIndicator *indicator;
    Gtk::Menu *menu;
    Gtk::ImageMenuItem *home_item = nullptr;
    Gtk::ImageMenuItem *quit_item = nullptr;
    Glib::RefPtr<Gtk::Application> app;
    WindowSize win_size;
    LocalizationManager &localization_Manager;

protected:
    void on_show();
    void on_quit();
    void listener_update_ui() override;
};

#endif // APP_INDICATOR_STATUS_H
