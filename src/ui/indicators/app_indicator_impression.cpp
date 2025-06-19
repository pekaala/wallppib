#include "../../../include/ui/indicators/app_indicator_impression.h"

using namespace std;

AppIndicatorImpression::AppIndicatorImpression(LocalizationManager &locManager, Glib::RefPtr<Gtk::Application> app_ref) : localization_Manager(locManager), app(app_ref)
{
    set_default_size(300, 200);

    string iconPath = DirectoryPath::get_data_icon_path();

    indicator = app_indicator_new(
        "my-app-indicator",
        (iconPath + "/indi_wllp.svg").c_str(),
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    menu = Gtk::make_managed<Gtk::Menu>();
    menu->set_name("app-indicator-menu");

    auto header = Gtk::make_managed<Gtk::MenuItem>(win_size.appName);
    header->set_sensitive(false);
    menu->append(*header);

    /*menu->append(*Gtk::make_managed<Gtk::SeparatorMenuItem>());
    home_item = Gtk::make_managed<Gtk::ImageMenuItem>(localization_Manager.get_key("menu_explore"));
    home_item->signal_activate().connect(sigc::mem_fun(*this, &AppIndicatorImpression::on_show));
    menu->append(*home_item);*/

    menu->append(*Gtk::make_managed<Gtk::SeparatorMenuItem>());

    quit_item = Gtk::make_managed<Gtk::ImageMenuItem>(localization_Manager.get_key("close"));
    quit_item->signal_activate().connect(
        sigc::mem_fun(*this, &AppIndicatorImpression::on_quit));
    menu->append(*quit_item);

    menu->show_all();
    app_indicator_set_menu(indicator, GTK_MENU(menu->gobj()));
}

AppIndicatorImpression::~AppIndicatorImpression()
{
}

void AppIndicatorImpression::on_show()
{
}

void AppIndicatorImpression::on_quit()
{
    system("killall xwinwrap gifview");
    app->get_default()->quit();
}

void AppIndicatorImpression::listener_update_ui()
{
    // home_item->set_label(localization_Manager.get_key("menu_explore"));
    quit_item->set_label(localization_Manager.get_key("close"));
}