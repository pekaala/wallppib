#include "../../include/ui/app_bar_window.h"

AppBarWindow::AppBarWindow(WindowSize winSize, Glib::RefPtr<Gtk::Application> app_ref) : m_VBox(Gtk::ORIENTATION_VERTICAL), m_Notebook(), app(app_ref)
{
    set_title(winSize.appName);
    set_default_size(winSize.W, winSize.H);
    signal_delete_event().connect(sigc::mem_fun(*this, &AppBarWindow::send_background_app));

    apply_header_bar_menu();

    m_VBox.pack_start(m_Notebook, Gtk::PACK_EXPAND_WIDGET);

    string iconPath = DirectoryPath::get_data_icon_path();
    set_icon_from_file(iconPath + "/indi_wllp.svg");

    about_window = new AboutWindow(localization_Manager);
    home_window = new HomeWindow(localization_Manager);
    settings_window = new SettingsWindow(localization_Manager);
    indicator = new AppIndicatorImpression(localization_Manager, app);

    localization_Manager.register_localizable_window(this);
    localization_Manager.register_localizable_window(about_window);
    localization_Manager.register_localizable_window(home_window);
    localization_Manager.register_localizable_window(settings_window);
    localization_Manager.register_localizable_window(indicator);

    m_Notebook.append_page(home_window->get_home_window_box(), localization_Manager.get_key("menu_explore"));

    add(m_VBox);
    show_all_children();
}

AppBarWindow::~AppBarWindow()
{
}

void AppBarWindow::apply_header_bar_menu()
{
    header_Bar.set_show_close_button(true);
    set_titlebar(header_Bar);

    menu_About.set_label(localization_Manager.get_key("menu_about"));
    menu_Settings.set_label(localization_Manager.get_key("menu_settings"));
    menu_Quit.set_label(localization_Manager.get_key("menu_quit"));

    menu.append(menu_About);
    menu.append(menu_Settings);
    menu.append(menu_Quit);

    menu_About.show();
    menu_Settings.show();
    menu_Quit.show();

    menu_About.signal_activate().connect(sigc::mem_fun(*this, &AppBarWindow::on_about_dialog));

    menu_Settings.signal_activate().connect(sigc::mem_fun(*this, &AppBarWindow::on_settings_dialog));

    menu_Quit.signal_activate()
        .connect([this]()
                 { hide(); });

    string iconPath = DirectoryPath::get_data_icon_path();

    auto icon = Gtk::manage(new Gtk::Image(iconPath + "/ICmenu.png"));

    menu_button.get_style_context()->add_class("outlined-button");
    menu_button.set_image(*icon);
    menu_button.signal_clicked().connect([this]()
                                         { this->menu.popup_at_pointer(nullptr); });

    header_Bar.pack_end(menu_button);
}

void AppBarWindow::on_about_dialog()
{
    about_window->show_about_dialog(*this);
}

void AppBarWindow::on_settings_dialog()
{
    settings_window->show_settings_dialog(*this);
}

bool AppBarWindow::send_background_app(GdkEventAny *event)
{
    auto app = Glib::RefPtr<Gtk::Application>::cast_dynamic(get_application());
    app->hold();
    hide();
    return true;
}

void AppBarWindow::listener_update_ui()
{
    m_Notebook.set_tab_label_text(home_window->get_home_window_box(), localization_Manager.get_key("menu_explore"));

    menu_About.set_label(localization_Manager.get_key("menu_about"));
    menu_Settings.set_label(localization_Manager.get_key("menu_settings"));
    menu_Quit.set_label(localization_Manager.get_key("menu_quit"));
}