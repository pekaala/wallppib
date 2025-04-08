#ifndef SETTINGS_VIEW_H
#define SETTINGS_VIEW_H

#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>
#include <giomm/menu.h>
#include <iostream>
#include "../localization/LocalizationManager.h"
#include "../localization/ILocalization.h"
#include "../Log.h"

class SettingsView : public ILocalization
{
public:
    SettingsView(LocalizationManager &locManager)
        : localization_Manager(locManager)
    {
        settingsViewBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
        settingsViewBox.set_spacing(25);
        settingsViewBox.set_halign(Gtk::ALIGN_CENTER);

        Gtk::Box *spacer = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        spacer->set_size_request(-1, 100);
        settingsViewBox.pack_start(*spacer, Gtk::PACK_SHRINK);

        for (const auto &[code, name] : localization_Manager.GetLanguages())
        {
            combo_Box.append(code, name);
        }

        combo_Box.set_size_request(300, -1);
        combo_Box.set_active_id(localization_Manager.GetSelectedLanguageCode());
        combo_Box.signal_changed().connect(sigc::mem_fun(*this, &SettingsView::on_language_changed));

        combo_Wrapper.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        combo_Wrapper.set_halign(Gtk::ALIGN_CENTER);
        combo_Wrapper.pack_start(combo_Box, Gtk::PACK_SHRINK);

        settingsViewBox.pack_start(combo_Wrapper, Gtk::PACK_SHRINK);
    }

    Gtk::Box &Get_Box_Settings()
    {
        return settingsViewBox;
    }

private:
    Gtk::Box settingsViewBox{Gtk::ORIENTATION_VERTICAL};
    Gtk::ComboBoxText combo_Box;
    Gtk::Box combo_Wrapper{Gtk::ORIENTATION_HORIZONTAL};
    LocalizationManager &localization_Manager;

    void on_language_changed()
    {
        std::string selectedCode = combo_Box.get_active_id();
        localization_Manager.ChangeLanguage(selectedCode);
    }

    void listener_update_ui() override
    {
    }
};

#endif // SETTINGS_VIEW_H
