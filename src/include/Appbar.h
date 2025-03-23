#ifndef APPBAR_H
#define APPBAR_H

#include <gtkmm/notebook.h>
#include <gtkmm/label.h>
#include "Explore.h"
#include "About.h"
#include "Windowsize.h"

class AppBar : public Gtk::Window
{
public:
    AppBar(windowsize winSize)
        : m_VBox(Gtk::ORIENTATION_VERTICAL), m_AppBarBox(Gtk::ORIENTATION_HORIZONTAL), m_Notebook()
    {
        set_title(winSize.appName);
        set_default_size(winSize.W, winSize.H);

        Gtk::MenuItem *exploreItem = Gtk::manage(new Gtk::MenuItem("Explore"));
        Gtk::MenuItem *aboutItem = Gtk::manage(new Gtk::MenuItem("About"));

        m_Menubar.append(*exploreItem);
        m_Menubar.append(*aboutItem);

        exploreItem->signal_button_press_event().connect(sigc::mem_fun(*this, &AppBar::on_explore_click));
        aboutItem->signal_button_press_event().connect(sigc::mem_fun(*this, &AppBar::on_about_click));

        m_VBox.pack_start(m_Menubar, Gtk::PACK_SHRINK);
        m_VBox.pack_start(m_Notebook, Gtk::PACK_EXPAND_WIDGET);

        m_ExploreWindow = new ExploreWindow();
        m_AboutWindow = new AboutWindow();

        m_Notebook.append_page(m_ExploreWindow->get_layout(), "Explore");
        m_Notebook.append_page(m_AboutWindow->get_box(), "About");

        add(m_VBox);
        show_all_children();
    }

protected:
    Gtk::MenuBar m_Menubar;
    Gtk::Box m_VBox;
    Gtk::Box m_AppBarBox;
    Gtk::Notebook m_Notebook;
    ExploreWindow *m_ExploreWindow;
    AboutWindow *m_AboutWindow;

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
};
#endif // APPBAR_H