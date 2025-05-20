#include "../../../include/ui/alerts/mesage.h"

ToastMessage::ToastMessage(const Glib::ustring &message)
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

    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &ToastMessage::on_timeout), 3800);
}

ToastMessage::~ToastMessage()
{
}

void ToastMessage::on_timeout()
{
    hide();
}