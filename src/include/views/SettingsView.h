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
#include "../settings/Settings.h"

using namespace std;

class SettingsView : public ILocalization
{
public:
    SettingsView(LocalizationManager &locManager) : localization_Manager(locManager) {}

    void Show_Settings_Dialog(Gtk::Window &parent)
    {
        if (dialog)
        {
            delete dialog;
            dialog = nullptr;
        }

        dialog = new Gtk::Dialog("", parent, true);
        dialog->set_title(localization_Manager.GetKey("dialog_title_settings"));
        dialog->set_modal(true);
        dialog->set_default_size(350, 180);
        dialog->set_border_width(20);
        dialog->set_resizable(false);

        Gtk::Box *content = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 15));
        content->set_halign(Gtk::ALIGN_FILL);
        content->set_valign(Gtk::ALIGN_CENTER);

        language_Label.set_text(localization_Manager.GetKey("select_lang"));
        language_Label.set_halign(Gtk::ALIGN_START);
        language_Label.set_margin_bottom(5);
        content->pack_start(language_Label, Gtk::PACK_SHRINK);

        combo_Box.set_size_request(250, -1);
        combo_Box.set_margin_bottom(10);

        if (combo_Box.get_model() && combo_Box.get_model()->children().empty())
        {
            for (const auto &[code, name] : localization_Manager.GetLanguages())
            {
                combo_Box.append(code, name);
            }
        }

        combo_Box.set_active_id(localization_Manager.GetSelectedLanguageCode());
        combo_Box.signal_changed().connect(sigc::mem_fun(*this, &SettingsView::on_language_changed));

        combo_Wrapper.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        combo_Wrapper.set_halign(Gtk::ALIGN_CENTER);
        combo_Wrapper.pack_start(combo_Box, Gtk::PACK_SHRINK);
        content->pack_start(combo_Wrapper, Gtk::PACK_SHRINK);

        dialog->get_content_area()->pack_start(*content);
        dialog->get_content_area()->show_all();

        Gtk::Box *switch_Wrapper = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
        switch_Wrapper->set_halign(Gtk::ALIGN_CENTER);
        switch_Wrapper->set_margin_bottom(20);

        switch_Label.set_text(localization_Manager.GetKey("automatically_resize_image"));
        switch_Label.set_halign(Gtk::ALIGN_START);

        auto_Convert_Switch.set_active(Settings::GetSettingsValue());
        auto_Convert_Switch.property_active().signal_changed().connect(sigc::mem_fun(*this, &SettingsView::on_settings_changed));

        switch_Wrapper->pack_start(switch_Label, Gtk::PACK_SHRINK);
        switch_Wrapper->pack_start(auto_Convert_Switch, Gtk::PACK_SHRINK);

        content->pack_start(*switch_Wrapper, Gtk::PACK_SHRINK);

        dialog->get_content_area()->pack_start(*content);
        dialog->get_content_area()->show_all();

        close_Button.set_label(localization_Manager.GetKey("close"));
        close_Button.show();
        dialog->add_action_widget(close_Button, Gtk::RESPONSE_CLOSE);

        dialog->run();

        delete dialog;
        dialog = nullptr;
    }

private:
    Gtk::Dialog *dialog = nullptr;
    Gtk::Label language_Label;
    Gtk::Label switch_Label;
    Gtk::Button close_Button;
    Gtk::ComboBoxText combo_Box;
    Gtk::Box combo_Wrapper{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Switch auto_Convert_Switch;
    LocalizationManager &localization_Manager;

    void on_language_changed()
    {
        string selectedCode = combo_Box.get_active_id();
        localization_Manager.ChangeLanguage(selectedCode);
    }

    void on_settings_changed()
    {
        Settings::OnSettingsChanged(auto_Convert_Switch.get_active());
    }

    void listener_update_ui() override
    {
        dialog->set_title(localization_Manager.GetKey("dialog_title_settings"));
        language_Label.set_text(localization_Manager.GetKey("select_lang"));
        switch_Label.set_text(localization_Manager.GetKey("automatically_resize_image"));
        close_Button.set_label(localization_Manager.GetKey("close"));
    }
};

#endif // SETTINGS_VIEW_H
