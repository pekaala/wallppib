#include "../../include/ui/settings_window.h"

using namespace std;

SettingsWindow::SettingsWindow(LocalizationManager &locManager) : localization_Manager(locManager)
{
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::show_settings_dialog(Gtk::Window &parent)
{
    if (dialog)
    {
        delete dialog;
        dialog = nullptr;
    }

    dialog = new Gtk::Dialog("", parent, true);
    dialog->set_title(localization_Manager.get_key("dialog_title_settings"));
    dialog->set_modal(true);
    dialog->set_default_size(350, 180);
    dialog->set_border_width(20);
    dialog->set_resizable(false);

    Gtk::Box *content = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 15));
    content->set_halign(Gtk::ALIGN_FILL);
    content->set_valign(Gtk::ALIGN_CENTER);

    language_Label.set_text(localization_Manager.get_key("select_lang"));
    language_Label.set_halign(Gtk::ALIGN_START);
    language_Label.set_margin_bottom(5);
    content->pack_start(language_Label, Gtk::PACK_SHRINK);

    combo_Box.set_size_request(250, -1);
    combo_Box.set_margin_bottom(10);
    combo_Box.get_style_context()->add_class("outlined-button");

    if (combo_Box.get_model() && combo_Box.get_model()->children().empty())
    {
        for (const auto &[code, name] : localization_Manager.get_languages())
        {
            combo_Box.append(code, name);
        }
    }

    combo_Box.set_active_id(localization_Manager.get_selected_language_code());
    combo_Box.signal_changed().connect(sigc::mem_fun(*this, &SettingsWindow::on_language_changed));

    combo_Wrapper.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    combo_Wrapper.set_halign(Gtk::ALIGN_CENTER);
    combo_Wrapper.pack_start(combo_Box, Gtk::PACK_SHRINK);
    content->pack_start(combo_Wrapper, Gtk::PACK_SHRINK);

    dialog->get_content_area()->pack_start(*content);
    dialog->get_content_area()->show_all();

    Gtk::Box *switch_Wrapper = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
    switch_Wrapper->set_halign(Gtk::ALIGN_CENTER);
    switch_Wrapper->set_margin_bottom(20);

    switch_Label.set_text(localization_Manager.get_key("automatically_resize_image"));
    switch_Label.set_halign(Gtk::ALIGN_START);

    auto_Convert_Switch.set_active(Settings::read_settings_config());
    auto_Convert_Switch.property_active().signal_changed().connect(sigc::mem_fun(*this, &SettingsWindow::on_settings_changed));

    switch_Wrapper->pack_start(switch_Label, Gtk::PACK_SHRINK);
    switch_Wrapper->pack_start(auto_Convert_Switch, Gtk::PACK_SHRINK);

    content->pack_start(*switch_Wrapper, Gtk::PACK_SHRINK);

    dialog->get_content_area()->pack_start(*content);
    dialog->get_content_area()->show_all();

    close_Button.set_label(localization_Manager.get_key("close"));
    close_Button.get_style_context()->add_class("outlined-button");
    close_Button.show();
    dialog->add_action_widget(close_Button, Gtk::RESPONSE_CLOSE);

    dialog->run();

    delete dialog;
    dialog = nullptr;
}

void SettingsWindow::on_language_changed()
{
    string selectedCode = combo_Box.get_active_id();
    localization_Manager.change_language(selectedCode);
}

void SettingsWindow::on_settings_changed()
{
    Settings::handle_settings_toggle(auto_Convert_Switch.get_active());
}

void SettingsWindow::listener_update_ui()
{
    dialog->set_title(localization_Manager.get_key("dialog_title_settings"));
    language_Label.set_text(localization_Manager.get_key("select_lang"));
    switch_Label.set_text(localization_Manager.get_key("automatically_resize_image"));
    close_Button.set_label(localization_Manager.get_key("close"));
}