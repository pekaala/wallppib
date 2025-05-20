#ifndef TOAST_MESSAGE_H
#define TOAST_MESSAGE_H

#include <gtkmm.h>
#include <thread>
#include <chrono>

class ToastMessage : public Gtk::Window
{

public:
    ToastMessage(const Glib::ustring &message);
    ~ToastMessage();

protected:
    void on_timeout();
};

#endif // TOAST_MESSAGE_H
