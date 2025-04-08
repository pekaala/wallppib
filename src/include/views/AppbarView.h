#ifndef APPBAR_VIEW_H
#define APPBAR_VIEW_H

#include <gtkmm/notebook.h>
#include <gtkmm/label.h>
#include <gtkmm/iconinfo.h>
#include "ExploreView.h"
#include "AboutView.h"
#include "SettingsView.h"
#include "../Windowsize.h"
#include "../localization/LocalizationManager.h"
#include "../Log.h"

class AppbarView : public Gtk::Window, public ILocalization
{
public:
    AppbarView(Windowsize winSize)
        : m_VBox(Gtk::ORIENTATION_VERTICAL), m_AppBarBox(Gtk::ORIENTATION_HORIZONTAL), m_Notebook()
    {
        set_title(winSize.appName);
        set_default_size(winSize.W, winSize.H);

        /*m_exploreItem = Gtk::manage(new Gtk::MenuItem(localization_Manager.GetKey("menu_explore")));
        m_aboutItem = Gtk::manage(new Gtk::MenuItem(localization_Manager.GetKey("menu_about")));

        m_Menubar.append(*m_exploreItem);
        m_Menubar.append(*m_aboutItem);

        m_exploreItem->signal_button_press_event().connect(sigc::mem_fun(*this, &AppbarView::on_explore_click));
        m_aboutItem->signal_button_press_event().connect(sigc::mem_fun(*this, &AppbarView::on_about_click));*/

        // m_VBox.pack_start(m_Menubar, Gtk::PACK_SHRINK);
        m_VBox.pack_start(m_Notebook, Gtk::PACK_EXPAND_WIDGET);

        m_ExploreView = new ExploreView(localization_Manager);
        m_AboutView = new AboutView(localization_Manager);
        m_SettingsView = new SettingsView(localization_Manager);

        localization_Manager.RegisterLocalizableView(this);
        localization_Manager.RegisterLocalizableView(m_ExploreView);
        localization_Manager.RegisterLocalizableView(m_AboutView);
        localization_Manager.RegisterLocalizableView(m_SettingsView);

        m_Notebook.append_page(m_ExploreView->Get_Layout_Explore(), localization_Manager.GetKey("menu_explore"));
        m_Notebook.append_page(m_AboutView->Get_Box_About(), localization_Manager.GetKey("menu_about"));
        m_Notebook.append_page(m_SettingsView->Get_Box_Settings(), localization_Manager.GetKey("menu_settings"));

        add(m_VBox);
        show_all_children();
    }

private:
    Gtk::MenuBar m_Menubar;
    Gtk::MenuItem *m_exploreItem;
    Gtk::MenuItem *m_aboutItem;
    Gtk::Box m_VBox;
    Gtk::Box m_AppBarBox;
    Gtk::Notebook m_Notebook;
    ExploreView *m_ExploreView;
    AboutView *m_AboutView;
    Gtk::MenuItem m_ExploreItem;
    SettingsView *m_SettingsView;
    LocalizationManager localization_Manager;

    bool on_explore_click(GdkEventButton *event)
    {
        if (event->type == GDK_BUTTON_PRESS && event->button == 1)
        {
            m_Notebook.set_current_page(0);
            return true;
        }
        return false;
    }

    bool on_about_click(GdkEventButton *event)
    {
        if (event->type == GDK_BUTTON_PRESS && event->button == 1)
        {
            m_Notebook.set_current_page(1);
            return true;
        }
        return false;
    }

    void listener_update_ui() override
    {
        m_Notebook.set_tab_label_text(m_ExploreView->Get_Layout_Explore(), localization_Manager.GetKey("menu_explore"));
        m_Notebook.set_tab_label_text(m_AboutView->Get_Box_About(), localization_Manager.GetKey("menu_about"));
        m_Notebook.set_tab_label_text(m_SettingsView->Get_Box_Settings(), localization_Manager.GetKey("menu_settings"));

        /*m_exploreItem->set_label(localization_Manager.GetKey("menu_explore"));
        m_aboutItem->set_label(localization_Manager.GetKey("menu_about"));*/
    }
};
#endif // APPBAR_VIEW_H