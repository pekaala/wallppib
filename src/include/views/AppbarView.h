#ifndef APPBAR_VIEW_H
#define APPBAR_VIEW_H

#include <gtkmm/notebook.h>
#include <gtkmm/label.h>
#include <gtkmm/iconinfo.h>
#include "ExploreView.h"
#include "AboutView.h"
#include "SettingsView.h"
#include "../common/WindowSize.h"
#include "../localization/LocalizationManager.h"
#include "../Log.h"
#include "../DirectoryPath.h"

using namespace std;

class AppbarView : public Gtk::Window, public ILocalization
{
public:
    AppbarView(WindowSize winSize)
        : m_VBox(Gtk::ORIENTATION_VERTICAL), m_AppBarBox(Gtk::ORIENTATION_HORIZONTAL), m_Notebook()
    {
        set_title(winSize.appName);
        set_default_size(winSize.W, winSize.H);

        m_VBox.pack_start(m_Notebook, Gtk::PACK_EXPAND_WIDGET);

        m_ExploreView = new ExploreView(localization_Manager);
        m_AboutView = new AboutView(localization_Manager);
        m_SettingsView = new SettingsView(localization_Manager);

        localization_Manager.RegisterLocalizableView(this);
        localization_Manager.RegisterLocalizableView(m_ExploreView);
        localization_Manager.RegisterLocalizableView(m_AboutView);
        localization_Manager.RegisterLocalizableView(m_SettingsView);

        m_Notebook.append_page(m_ExploreView->Get_Layout_Explore(), localization_Manager.GetKey("menu_explore"));

        header_bar();

        add(m_VBox);
        show_all_children();
    }

private:
    Gtk::HeaderBar header_Bar;
    Gtk::Button menu_button;
    Gtk::Menu menu;
    Gtk::MenuItem menu_About, menu_Settings, menu_Quit;
    Gtk::MenuBar m_Menubar;
    Gtk::MenuItem *m_aboutItem;
    Gtk::Box m_VBox;
    Gtk::Box m_AppBarBox;
    Gtk::Notebook m_Notebook;
    ExploreView *m_ExploreView;
    AboutView *m_AboutView;
    SettingsView *m_SettingsView;
    LocalizationManager localization_Manager;

    void header_bar()
    {
        header_Bar.set_show_close_button(true);
        set_titlebar(header_Bar);

        menu_About.set_label(localization_Manager.GetKey("menu_about"));
        menu_Settings.set_label(localization_Manager.GetKey("menu_settings"));
        menu_Quit.set_label(localization_Manager.GetKey("menu_quit"));

        menu.append(menu_About);
        menu.append(menu_Settings);
        menu.append(menu_Quit);

        menu_About.show();
        menu_Settings.show();
        menu_Quit.show();

        menu_About.signal_activate().connect(sigc::mem_fun(*this, &AppbarView::on_about_dialog));

        menu_Settings.signal_activate().connect(sigc::mem_fun(*this, &AppbarView::on_settings_dialog));

        menu_Quit.signal_activate()
            .connect([this]()
                     { hide(); });

        string iconPath = DirectoryPath::GetDataIconPath();

        auto icon = Gtk::manage(new Gtk::Image(iconPath + "/ICmenu.png"));

        menu_button.set_image(*icon);
        menu_button.signal_clicked().connect([this]()
                                             { this->menu.popup_at_pointer(nullptr); });

        header_Bar.pack_end(menu_button);
    }

    void on_about_dialog()
    {
        m_AboutView->Show_About_Dialog(*this);
    }

    void on_settings_dialog()
    {
        m_SettingsView->Show_Settings_Dialog(*this);
    }

    void listener_update_ui() override
    {
        m_Notebook.set_tab_label_text(m_ExploreView->Get_Layout_Explore(), localization_Manager.GetKey("menu_explore"));

        menu_About.set_label(localization_Manager.GetKey("menu_about"));
        menu_Settings.set_label(localization_Manager.GetKey("menu_settings"));
        menu_Quit.set_label(localization_Manager.GetKey("menu_quit"));
    }
};
#endif // APPBAR_VIEW_H