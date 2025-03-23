#ifndef ABOUT_H
#define ABOUT_H

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/separator.h>
#include <gtkmm/image.h>
#include "DirectoryPath.h"

class AboutWindow
{
public:
    AboutWindow()
    {
        m_AboutBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
        m_AboutBox.set_spacing(10);
        m_AboutBox.set_halign(Gtk::ALIGN_CENTER);

        m_LogoBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_LogoBox.set_spacing(10);
        m_LogoBox.set_halign(Gtk::ALIGN_CENTER);

        std::string iconStr = directory_Path.getDataIcon();

        Glib::RefPtr<Gdk::Pixbuf> logoPixbuf = Gdk::Pixbuf::create_from_file(iconStr + "/logo_icon.svg");
        Glib::RefPtr<Gdk::Pixbuf> scaledPixbuf = logoPixbuf->scale_simple(250, 100, Gdk::INTERP_BILINEAR);

        m_Logo.set(scaledPixbuf);
        m_Logo.set_pixel_size(25);

        m_AboutBox.pack_start(m_Logo, Gtk::PACK_SHRINK);

        m_VersionLabel.set_text("Version: " + win_Size.version);
        m_VersionLabel.set_halign(Gtk::ALIGN_CENTER);
        m_AboutBox.pack_start(m_VersionLabel, Gtk::PACK_SHRINK);

        m_ButtonBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_ButtonBox.set_spacing(10);
        m_ButtonBox.set_halign(Gtk::ALIGN_CENTER);

        /*buttonDonate.set_label("Donate");
        buttonDonate.signal_clicked().connect(sigc::mem_fun(*this, &AboutWindow::on_donate_clicked));
        m_ButtonBox.pack_start(buttonDonate, Gtk::PACK_SHRINK);*/

        /*
        buttonFeedback.set_label("Give Feedback");
        buttonFeedback.signal_clicked().connect(sigc::mem_fun(*this, &AboutWindow::on_feedback_clicked));
        buttonFeedback.set_sensitive(false);
        m_ButtonBox.pack_start(buttonFeedback, Gtk::PACK_SHRINK);*/

        buttonSourceCode.set_label("Github Source Code ");
        buttonSourceCode.signal_clicked().connect(sigc::mem_fun(*this, &AboutWindow::on_source_code_clicked));
        m_ButtonBox.pack_start(buttonSourceCode, Gtk::PACK_SHRINK);

        m_AboutBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

        m_Separator.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_AboutBox.pack_start(m_Separator, Gtk::PACK_SHRINK);
    }

    Gtk::Box &get_box()
    {
        return m_AboutBox;
    }

private:
    Gtk::Box m_AboutBox;
    Gtk::Box m_LogoBox;
    Gtk::Box m_ButtonBox;
    Gtk::Label m_VersionLabel;
    Gtk::Button buttonDonate;
    Gtk::Button buttonFeedback;
    Gtk::Button buttonSourceCode;
    Gtk::Image m_Logo;
    Gtk::Separator m_Separator;
    windowsize win_Size;
    DirectoryPath directory_Path;

    void on_donate_clicked()
    {
        system("xdg-open https://www.example.com/donate");
    }

    void on_feedback_clicked()
    {
        system("xdg-open https://www.example.com/feedback");
    }

    void on_source_code_clicked()
    {
        system("xdg-open https://github.com/pekaala/wallppib");
    }
};

#endif // ABOUT_H
