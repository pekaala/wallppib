#include "../../include/ui/home_window.h"

/*PUBLIC*/
HomeWindow::HomeWindow(LocalizationManager &locManager) : localization_Manager(locManager)
{
    main_Box.set_orientation(Gtk::ORIENTATION_VERTICAL);

    show_error_message_x11();

    auto button_hbox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 10);
    button_hbox->set_margin_top(10);
    button_hbox->set_margin_start(45);
    button_hbox->set_margin_bottom(5);
    button_hbox->set_halign(Gtk::ALIGN_START);

    auto icon = Gtk::make_managed<Gtk::Image>(DirectoryPath::get_data_icon_path() + "/ICplus.png");
    auto select_File_Button = Gtk::manage(new Gtk::Button());
    select_File_Button->set_image(*icon);
    select_File_Button->set_size_request(80, 25);
    select_File_Button->get_style_context()->add_class("outlined-button");
    select_File_Button->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::on_select_image));
    button_hbox->pack_start(*select_File_Button, Gtk::PACK_SHRINK);

    combo_Box.append("all", localization_Manager.get_key("filter_all"));
    combo_Box.append("jpg", "JPG");
    combo_Box.append("png", "PNG");
    combo_Box.append("gif", "GIF");
    combo_Box.set_active_id("all");
    combo_Box.set_size_request(80, 25);
    combo_Box.get_style_context()->add_class("outlined-button");
    combo_Box.property_active().signal_changed().connect(sigc::mem_fun(*this, &HomeWindow::on_change_filter));
    button_hbox->pack_start(combo_Box, Gtk::PACK_SHRINK);

    main_Box.pack_start(*button_hbox, Gtk::PACK_SHRINK);

    flowbox.set_valign(Gtk::ALIGN_START);
    flowbox.set_max_children_per_line(-1);
    flowbox.set_selection_mode(Gtk::SELECTION_NONE);
    flowbox.set_homogeneous(true);
    flowbox.set_row_spacing(10);
    flowbox.set_column_spacing(15);

    loading_animation_provider(false);
}

HomeWindow::~HomeWindow()
{
}

Gtk::Box &HomeWindow::get_home_window_box()
{
    return main_Box;
}

/*PROTECTED*/
void HomeWindow::loading_animation_provider(bool loadtype)
{
    clear_info_bar();
    loading_image.set(DirectoryPath::get_data_icon_path() + "/loading.gif");
    image_wrapper.add(loading_image);
    image_wrapper.set_visible(true);
    main_Box.pack_start(image_wrapper, Gtk::PACK_SHRINK);

    string selected_filter = combo_Box.get_active_id();

    thread([this, selected_filter, loadtype]()
           {
        image_Manager.get_all_images(selected_filter);

        Glib::signal_idle().connect_once([this, loadtype]() {
            load_images(loadtype); 
            main_Box.show_all_children();
            image_wrapper.set_visible(false);
        }); })
        .detach();
}

void HomeWindow::load_images(bool loadtype)
{
    if (loadtype)
    {
        for (auto *child : flowbox.get_children())
            flowbox.remove(*child);
    }

    int image_width = 540, image_height = 304;

    for (const auto &imageInfo : image_Manager.Images)
    {
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(imageInfo.get_path());
        Glib::RefPtr<Gdk::Pixbuf> scaled = pixbuf->scale_simple(image_width, image_height, Gdk::INTERP_BILINEAR);

        auto img = Gtk::make_managed<Gtk::Image>(scaled);
        img->set_halign(Gtk::ALIGN_CENTER);
        img->set_valign(Gtk::ALIGN_CENTER);

        auto type_label = Gtk::make_managed<Gtk::Label>(imageInfo.get_type());
        type_label->get_style_context()->add_class("image-type-label");
        type_label->get_style_context()->add_class(imageInfo.get_type());
        type_label->set_halign(Gtk::ALIGN_START);
        type_label->set_margin_top(4);
        type_label->set_margin_start(6);
        type_label->set_margin_bottom(4);

        auto image_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        image_box->pack_start(*img, Gtk::PACK_SHRINK);
        image_box->pack_start(*type_label, Gtk::PACK_SHRINK);

        auto frame = Gtk::make_managed<Gtk::Frame>();
        frame->set_shadow_type(Gtk::SHADOW_NONE);
        frame->set_margin_top(5);
        frame->set_margin_bottom(5);
        frame->set_size_request(image_width + 10, image_height + 30);
        frame->get_style_context()->add_class("outlined");
        frame->add(*image_box);

        auto eventBox = Gtk::make_managed<Gtk::EventBox>();
        eventBox->add(*frame);
        eventBox->signal_button_press_event().connect(
            sigc::bind(sigc::mem_fun(*this, &HomeWindow::on_image_click), imageInfo));

        flowbox.add(*eventBox);
    }

    if (!loadtype)
    {
        auto inner_vbox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
        inner_vbox->set_margin_top(10);
        inner_vbox->pack_start(flowbox, Gtk::PACK_EXPAND_WIDGET);

        scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        scrolled_window.add(*inner_vbox);
        main_Box.pack_start(scrolled_window, Gtk::PACK_EXPAND_WIDGET);
    }
}

bool HomeWindow::on_image_click(GdkEventButton *event, const Image &imageInfo)
{
    dialog = new Gtk::Dialog("", true);
    dialog->set_title(localization_Manager.get_key("select_image_info"));
    dialog->set_modal(true);
    dialog->set_default_size(600, 450);
    dialog->set_resizable(false);

    selected_Image_Path = imageInfo.get_path();

    Gtk::Box *mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12));
    dialog->get_content_area()->pack_start(*mainBox, Gtk::PACK_EXPAND_WIDGET);

    auto pixbuf = Gdk::Pixbuf::create_from_file(imageInfo.get_path());
    auto scaled_pixbuf = pixbuf->scale_simple(480, 270, Gdk::INTERP_BILINEAR);
    auto img = Gtk::manage(new Gtk::Image(scaled_pixbuf));
    auto overlay = Gtk::manage(new Gtk::Overlay());
    overlay->set_margin_bottom(10);
    overlay->add(*img);
    mainBox->pack_start(*overlay, Gtk::PACK_SHRINK);

    Gtk::Frame *infoFrame = Gtk::manage(new Gtk::Frame(""));
    infoFrame->set_shadow_type(Gtk::SHADOW_IN);

    Gtk::Box *infoBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 9));
    infoBox->set_margin_top(10);
    infoBox->set_margin_bottom(10);
    infoBox->set_margin_start(25);
    infoBox->set_margin_end(15);

    auto labelName = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_name") + " : " + imageInfo.get_image_name()));
    labelName->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelName, Gtk::PACK_SHRINK);

    auto labelPath = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_path") + " : " + imageInfo.get_path()));
    labelPath->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelPath, Gtk::PACK_SHRINK);

    string typeValue = imageInfo.get_type();
    string typeLabelText = localization_Manager.get_key("image_type") + " : ";

    auto labelTitle = Gtk::manage(new Gtk::Label(typeLabelText));
    labelTitle->set_halign(Gtk::ALIGN_START);
    labelTitle->get_style_context()->add_class("image-type-title");

    auto labelTypeValue = Gtk::manage(new Gtk::Label(typeValue));
    labelTypeValue->set_halign(Gtk::ALIGN_START);
    labelTypeValue->get_style_context()->add_class("image-type-label");
    labelTypeValue->get_style_context()->add_class(typeValue);

    auto typeBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 4));
    typeBox->pack_start(*labelTitle, Gtk::PACK_SHRINK);
    typeBox->pack_start(*labelTypeValue, Gtk::PACK_SHRINK);

    infoBox->pack_start(*typeBox, Gtk::PACK_SHRINK);

    auto labelWidth = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_width") + " : " + to_string(imageInfo.get_width())));
    labelWidth->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelWidth, Gtk::PACK_SHRINK);

    auto labelHeight = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_height") + " : " + to_string(imageInfo.get_height())));
    labelHeight->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelHeight, Gtk::PACK_SHRINK);

    infoFrame->add(*infoBox);
    mainBox->pack_start(*infoFrame, Gtk::PACK_SHRINK);

    string iconPath = DirectoryPath::get_data_icon_path();

    auto buttonBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 4));

    auto wallpaper_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICwallpaper.png"));
    auto walllpaper_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("set_wallpaper")));

    auto contentBox1 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
    contentBox1->pack_start(*wallpaper_Btn_Icon, Gtk::PACK_SHRINK);
    contentBox1->pack_start(*walllpaper_Btn_Label, Gtk::PACK_SHRINK);

    auto walllpaper_btn = Gtk::manage(new Gtk::Button());
    walllpaper_btn->add(*contentBox1);
    walllpaper_btn->set_size_request(80, 30);
    walllpaper_btn->set_margin_start(15);
    buttonBox->pack_start(*walllpaper_btn, Gtk::PACK_SHRINK);

    auto open_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICimage_marker.png"));
    auto open_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_open_containing_file")));

    auto contentBox3 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
    contentBox3->pack_start(*open_Btn_Icon, Gtk::PACK_SHRINK);
    contentBox3->pack_start(*open_Btn_Label, Gtk::PACK_SHRINK);

    auto openFile_Btn = Gtk::manage(new Gtk::Button());
    openFile_Btn->add(*contentBox3);
    openFile_Btn->set_size_request(80, 30);
    openFile_Btn->set_margin_start(15);
    buttonBox->pack_start(*openFile_Btn, Gtk::PACK_SHRINK);

    auto delete_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICdelete.png"));
    auto delete_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_delete")));

    auto contentBox2 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
    contentBox2->pack_start(*delete_Btn_Icon, Gtk::PACK_SHRINK);
    contentBox2->pack_start(*delete_Btn_Label, Gtk::PACK_SHRINK);

    auto delete_Btn = Gtk::manage(new Gtk::Button());
    delete_Btn->add(*contentBox2);
    delete_Btn->set_size_request(80, 30);
    delete_Btn->set_margin_start(15);
    buttonBox->pack_start(*delete_Btn, Gtk::PACK_SHRINK);

    if (FileType::is_gif(imageInfo.get_type()))
    {
        auto preview_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICpreview.png"));
        auto preview_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("play_gif")));

        auto contentBox4 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
        contentBox4->pack_start(*preview_Btn_Icon, Gtk::PACK_SHRINK);
        contentBox4->pack_start(*preview_Btn_Label, Gtk::PACK_SHRINK);

        auto preview_Btn = Gtk::manage(new Gtk::Button());
        preview_Btn->add(*contentBox4);
        preview_Btn->get_style_context()->add_class("outlined-button");
        preview_Btn->set_size_request(80, 30);
        preview_Btn->set_margin_start(15);
        preview_Btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &HomeWindow::on_preview_gif), imageInfo.get_path()));
        buttonBox->pack_start(*preview_Btn, Gtk::PACK_SHRINK);
    }

    dialog->get_content_area()->pack_start(*buttonBox, Gtk::PACK_SHRINK);
    dialog->show_all();

    walllpaper_btn->get_style_context()->add_class("outlined-button");
    walllpaper_btn->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::on_set_wallpaper));
    openFile_Btn->get_style_context()->add_class("outlined-button");
    openFile_Btn->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::open_containing_current_file));
    delete_Btn->get_style_context()->add_class("outlined-button");
    delete_Btn->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::on_delete_image));

    auto close_Button = Gtk::make_managed<Gtk::Button>(localization_Manager.get_key("close"));
    close_Button->get_style_context()->add_class("outlined-button");
    close_Button->show();
    dialog->add_action_widget(*close_Button, Gtk::RESPONSE_CLOSE);

    dialog->run();

    delete dialog;
    dialog = nullptr;
    return true;
}

void HomeWindow::on_select_image()
{
    file_chooser_dialog = new Gtk::FileChooserDialog(localization_Manager.get_key("select_image"), Gtk::FILE_CHOOSER_ACTION_OPEN);

    auto close_Button = Gtk::make_managed<Gtk::Button>(localization_Manager.get_key("close"));
    close_Button->get_style_context()->add_class("outlined-button");
    close_Button->show();
    file_chooser_dialog->add_action_widget(*close_Button, Gtk::RESPONSE_CANCEL);

    auto open_Button = Gtk::make_managed<Gtk::Button>(localization_Manager.get_key("open"));
    open_Button->get_style_context()->add_class("outlined-button");
    open_Button->show();
    file_chooser_dialog->add_action_widget(*open_Button, Gtk::RESPONSE_OK);

    file_chooser_dialog->set_default_size(150, 200);
    file_chooser_dialog->set_resizable(false);
    file_chooser_dialog->set_border_width(20);

    auto filter_image = Gtk::FileFilter::create();
    filter_image->set_name(localization_Manager.get_key("image_files"));
    filter_image->add_mime_type("image/png");
    filter_image->add_mime_type("image/jpeg");
    filter_image->add_mime_type("image/gif");
    filter_image->add_pattern("*.png");
    filter_image->add_pattern("*.jpg");
    filter_image->add_pattern("*.jpeg");
    filter_image->add_pattern("*.gif");
    file_chooser_dialog->add_filter(filter_image);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name(localization_Manager.get_key("all_files"));
    filter_any->add_pattern("*");
    file_chooser_dialog->add_filter(filter_any);

    m_ProgressBar = new Gtk::ProgressBar();
    m_ProgressBar->set_text("");
    m_ProgressBar->set_show_text(true);
    m_ProgressBar->set_visible(false);
    m_ProgressBar->set_size_request(300, 30);
    m_ProgressBar->set_hexpand(true);
    m_ProgressBar->set_halign(Gtk::ALIGN_FILL);
    m_ProgressBar->get_style_context()->add_class("progressbar");

    file_chooser_dialog->signal_response().connect([=](int response_id)
                                                   {
        if (response_id == Gtk::RESPONSE_OK)
        {
            is_done=false;
        add_result = false;
        m_ProgressBar->set_visible(true);
        file_chooser_dialog->set_sensitive(false);

        Gtk::Box* vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));
        vbox->set_margin_top(20);
        vbox->set_margin_bottom(20);
        vbox->set_margin_start(30);
        vbox->set_margin_end(30);

        Gtk::Label* label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("loading_file")));
        label->set_halign(Gtk::ALIGN_CENTER);
        label->get_style_context()->add_class("bold-text");

        vbox->pack_start(*label, Gtk::PACK_SHRINK);
        vbox->pack_start(*m_ProgressBar, Gtk::PACK_SHRINK);

        file_chooser_dialog->get_vbox()->pack_start(*vbox, Gtk::PACK_SHRINK);
        file_chooser_dialog->show_all_children();

            auto children = file_chooser_dialog->get_vbox()->get_children();
            for (auto *child : children)
            {
                if (child != vbox)
                    child->hide();
            }

                string fileName = file_chooser_dialog->get_filename();

            thread([this, fileName]()
            {
                this_thread::sleep_for(chrono::seconds(2)); 
                auto future = image_Manager.add_image_async(fileName);
                bool result = future.get();

                    Glib::signal_idle().connect_once([this, result]()
                {
                    add_result = result;
                    is_done = true;
                   loading_animation_provider(true);
                });
            }).detach();

            if (!m_connection_timeout.connected())
            {
                m_connection_timeout = Glib::signal_timeout().connect(
                    sigc::mem_fun(*this, &HomeWindow::on_timeout),
                    50);
            }
    }
    else
    {
        file_chooser_dialog->hide();
        delete file_chooser_dialog;
    } });

    file_chooser_dialog->show();
}

void HomeWindow::on_set_wallpaper()
{
    clear_info_bar();

    dialog->hide();
    wallpaper_Manager.set_wallpaper(selected_Image_Path);

    auto *info_bar = new InfoBarMessage(localization_Manager.get_key("success"), "success");
    main_Box.pack_start(*info_bar, Gtk::PACK_SHRINK);
    info_bar->show_info_bar();
}

void HomeWindow::on_delete_image()
{
    clear_info_bar();
    bool delete_image_result = image_Manager.delete_image(selected_Image_Path);
    if (delete_image_result)
    {
        loading_animation_provider(true);
        dialog->hide();

        auto *info_bar = new InfoBarMessage(localization_Manager.get_key("success_delete_image"), "success");
        main_Box.pack_start(*info_bar, Gtk::PACK_SHRINK);
        info_bar->show_info_bar();
        return;
    }

    auto *info_bar = new InfoBarMessage(localization_Manager.get_key("error_delete_image"), "error");
    main_Box.pack_start(*info_bar, Gtk::PACK_SHRINK);
    info_bar->show_info_bar();
}

void HomeWindow::open_containing_current_file()
{
    DirectoryPath::open_containing_folder(selected_Image_Path);
}

void HomeWindow::on_close_dialog()
{
    dialog->hide();
}

void HomeWindow::listener_update_ui()
{
    auto model = combo_Box.get_model();

    for (auto row_iter = model->children().begin(); row_iter != model->children().end(); ++row_iter)
    {
        Gtk::TreeModel::Row row = *row_iter;

        Glib::ustring id;
        row.get_value(1, id);
        if (id == "all")
        {
            Glib::ustring new_label = localization_Manager.get_key("filter_all");
            row.set_value(0, new_label);
            break;
        }
    }
    x11_error_label.set_text(localization_Manager.get_key("error_x11"));
}

bool HomeWindow::on_timeout()
{
    if (is_done)
    {
        m_connection_timeout.disconnect();

        file_chooser_dialog->hide();
        delete file_chooser_dialog;
        load_images(true);
        return false;
    }

    m_ProgressBar->pulse();

    return true;
}

void HomeWindow::on_change_filter()
{
    loading_animation_provider(true);
}

void HomeWindow::on_preview_gif(string &filePath)
{
    auto dialog = new Gtk::Dialog(localization_Manager.get_key("playing"), true);
    dialog->set_default_size(300, 200);

    Glib::RefPtr<Gdk::PixbufAnimation> anim = Gdk::PixbufAnimation::create_from_file(filePath);

    auto image = Gtk::make_managed<Gtk::Image>(anim);
    image->set_halign(Gtk::ALIGN_CENTER);
    image->set_valign(Gtk::ALIGN_CENTER);
    dialog->get_content_area()->pack_start(*image);
    dialog->show_all_children();
    dialog->show();
}

void HomeWindow::show_error_message_x11()
{
    if (!SystemInfo::is_wayland())
    {
        auto infobox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 6));
        infobox->get_style_context()->add_class("custom-infobox");
        infobox->get_style_context()->add_class("warning");

        x11_error_label.set_text(localization_Manager.get_key("error_x11"));
        infobox->pack_start(x11_error_label, Gtk::PACK_SHRINK);
        main_Box.pack_start(*infobox, Gtk::PACK_SHRINK);
    }
}

void HomeWindow::clear_info_bar()
{
    for (auto *child : main_Box.get_children())
    {
        if (auto *info_bar = dynamic_cast<InfoBarMessage *>(child))
        {
            main_Box.remove(*info_bar);
            delete info_bar;
            break;
        }
    }
}