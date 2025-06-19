#ifndef INFO_BAR_MESSAGE_H
#define INFO_BAR_MESSAGE_H

#include <gtkmm.h>
#include <thread>
#include <chrono>
#include <string>

class InfoBarMessage : public Gtk::Box
{
public:
    InfoBarMessage(const Glib::ustring &message, const std::string &color);
    ~InfoBarMessage();
    void show_info_bar();

private:
    Gtk::Label m_label;
    sigc::connection m_timeout_conn;

    bool on_timeout();
};

#endif // INFO_BAR_MESSAGE_H
