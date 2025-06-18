#include "../../../include/ui/alerts/info_bar_message.h"

InfoBarMessage::InfoBarMessage(const Glib::ustring &message, const string &color)
{
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    get_style_context()->add_class("custom-infobox");
    get_style_context()->add_class(color);

    m_label.get_style_context()->add_class("bold-text");
    m_label.set_text(message);
    m_label.set_halign(Gtk::ALIGN_START);

    pack_start(m_label, Gtk::PACK_SHRINK);
}

InfoBarMessage::~InfoBarMessage()
{
}

void InfoBarMessage::show_info_bar()
{
    show();
    show_all_children();

    if (m_timeout_conn.connected())
        m_timeout_conn.disconnect();

    m_timeout_conn = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &InfoBarMessage::on_timeout),
        1500);
}

bool InfoBarMessage::on_timeout()
{
    hide();
    return false;
}
