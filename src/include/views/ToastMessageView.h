#ifndef TOAST_MESSAGE_VIEW_H
#define TOAST_MESSAGE_VIEW_H

#include <gtkmm.h>
#include <thread>
#include <chrono>

class ToastMessageView : public Gtk::Window
{
public:
    ToastMessageView(const Glib::ustring &message)
    {
        set_type_hint(Gdk::WINDOW_TYPE_HINT_POPUP_MENU);
        set_position(Gtk::WIN_POS_NONE);
        set_decorated(false);
        set_resizable(false);
        set_keep_above(true);
        set_default_size(200, 50);
        set_border_width(10);

        Gtk::Label *label = Gtk::manage(new Gtk::Label(message));
        add(*label);
        label->show();

        signal_realize().connect([this]()
                                 {
            auto display = Gdk::Display::get_default();
            if (display) {
                auto monitor = display->get_primary_monitor();
                if (monitor) {
                    Gdk::Rectangle geometry;
                    monitor->get_workarea(geometry);
                    move(geometry.get_x(), geometry.get_y()); 
                }
            } });

        show_all();

        Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &ToastMessageView::on_timeout), 3800);
    }

private:
    void on_timeout()
    {
        hide();
    }
};
#endif // TOAST_MESSAGE_VIEW_H
