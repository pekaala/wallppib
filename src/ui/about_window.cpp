#include "../../include/ui/about_window.h"

AboutWindow::AboutWindow(LocalizationManager &locManager) : localization_Manager(locManager)
{
}

AboutWindow::~AboutWindow()
{
}

void AboutWindow::show_about_dialog(Gtk::Window &parent)
{
    Gtk::Dialog dialog(localization_Manager.get_key("dialog_title_about"), parent, true);
    dialog.set_modal(true);
    dialog.set_default_size(420, 300);
    dialog.set_border_width(15);
    dialog.set_resizable(false);

    Gtk::Box *content = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));
    content->set_halign(Gtk::ALIGN_CENTER);
    content->set_valign(Gtk::ALIGN_CENTER);

    string iconPath = DirectoryPath::get_data_icon_path();
    Glib::RefPtr<Gdk::Pixbuf> logoPixbuf = Gdk::Pixbuf::create_from_file(iconPath + "/logo_icon.svg");
    Glib::RefPtr<Gdk::Pixbuf> scaledPixbuf = logoPixbuf->scale_simple(250, 100, Gdk::INTERP_BILINEAR);

    Gtk::Image *logoImage = Gtk::manage(new Gtk::Image(scaledPixbuf));
    content->pack_start(*logoImage, Gtk::PACK_SHRINK);

    Gtk::Button *githubButton = Gtk::manage(new Gtk::Button(localization_Manager.get_key("github_src")));
    githubButton->signal_clicked().connect(sigc::mem_fun(*this, &AboutWindow::on_source_code_clicked));
    githubButton->get_style_context()->add_class("outlined-button");
    content->pack_start(*githubButton, Gtk::PACK_SHRINK);

    Gtk::Button *donateButton = Gtk::manage(new Gtk::Button("Donate"));
    donateButton->signal_clicked().connect(sigc::mem_fun(*this, &AboutWindow::on_donate_clicked));
    // content->pack_start(*donateButton, Gtk::PACK_SHRINK);

    Gtk::Button *feedbackButton = Gtk::manage(new Gtk::Button("Give Feedback"));
    feedbackButton->signal_clicked().connect(sigc::mem_fun(*this, &AboutWindow::on_feedback_clicked));
    feedbackButton->set_sensitive(false);
    // content->pack_start(*feedbackButton, Gtk::PACK_SHRINK);

    string versionText = localization_Manager.get_key("app_version") + " " + win_Size.version;
    Gtk::Label *versionLabel = Gtk::manage(new Gtk::Label(versionText));
    versionLabel->set_halign(Gtk::ALIGN_CENTER);
    content->pack_start(*versionLabel, Gtk::PACK_SHRINK);

    dialog.get_content_area()->pack_start(*content);
    dialog.get_content_area()->show_all();

    close_Button.set_label(localization_Manager.get_key("close"));
    close_Button.get_style_context()->add_class("outlined-button");
    close_Button.show();
    dialog.add_action_widget(close_Button, Gtk::RESPONSE_CLOSE);

    dialog.run();
}

void AboutWindow::on_donate_clicked()
{
    // system("xdg-open https://www.example.com/donate");
}

void AboutWindow::on_feedback_clicked()
{
    // system("xdg-open https://www.example.com/feedback");
}

void AboutWindow::on_source_code_clicked()
{
    system("xdg-open https://github.com/pekaala/wallppib");
}

void AboutWindow::listener_update_ui()
{
}
