#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <giomm/menu.h>
#include <gtkmm/dialog.h>
#include <gtkmm/switch.h>
#include <iostream>
#include "../utils/localization/i_localization.h"
#include "../utils/localization/localization_manager.h"
#include "../utils/log.h"
#include "../utils/settings.h"

using namespace std;

class SettingsWindow : public ILocalization
{
public:
    SettingsWindow(LocalizationManager &locManager);
    ~SettingsWindow();

    void show_settings_dialog(Gtk::Window &parent);

private:
    Gtk::Dialog *dialog = nullptr;
    Gtk::Label language_Label;
    Gtk::Label switch_Label;
    Gtk::Button close_Button;
    Gtk::ComboBoxText combo_Box;
    Gtk::Box combo_Wrapper{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Switch auto_Convert_Switch;
    LocalizationManager &localization_Manager;

protected:
    void on_language_changed();
    void on_settings_changed();
    void listener_update_ui() override;
};

#endif // SETTINGS_WINDOW_H