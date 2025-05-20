#include "../../include/ui/home_window.h"

HomeWindow::HomeWindow(LocalizationManager &locManager) : localization_Manager(locManager)
{
    flowbox.set_valign(Gtk::ALIGN_START);
    flowbox.set_max_children_per_line(-1);
    flowbox.set_selection_mode(Gtk::SELECTION_NONE);
    flowbox.set_homogeneous(true);
    flowbox.set_row_spacing(10);
    flowbox.set_column_spacing(15);

    load_images(false);
    show_all_children();
}

Gtk::ScrolledWindow &HomeWindow::get_layout_scroll()
{
    return scrolled_window;
}

HomeWindow::~HomeWindow()
{
}

void HomeWindow::load_images(bool loadtype = false)
{
    if (loadtype)
    {
        auto children = flowbox.get_children();

        for (auto *child : children)
        {
            flowbox.remove(*child);
        }
    }

    int image_width = 540;
    int image_height = 304;

    image_Manager.GetImages();

    for (int i = 0; i < image_Manager.Images.size(); ++i)
    {
        const Image &imageInfo = image_Manager.Images[i];
        string image_path = imageInfo.getPath();

        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(image_path);
        Glib::RefPtr<Gdk::Pixbuf> scaled_pixbuf = pixbuf->scale_simple(image_width, image_height, Gdk::INTERP_BILINEAR);

        Gtk::Image *img = Gtk::manage(new Gtk::Image(scaled_pixbuf));
        img->set_halign(Gtk::ALIGN_CENTER);
        img->set_valign(Gtk::ALIGN_CENTER);

        Gtk::Frame *frame = Gtk::manage(new Gtk::Frame());
        frame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        frame->set_margin_top(10);
        frame->set_margin_bottom(10);
        frame->set_size_request(image_width + 10, image_height + 10);
        frame->add(*img);

        Gtk::EventBox *eventBox = Gtk::manage(new Gtk::EventBox());
        eventBox->add(*frame);

        eventBox->signal_button_press_event().connect(
            sigc::bind(sigc::mem_fun(*this, &HomeWindow::on_image_click), imageInfo));

        flowbox.add(*eventBox);
    }

    if (!loadtype)
    {
        auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 10);
        vbox->set_margin_top(10);
        vbox->set_margin_bottom(10);
        vbox->set_margin_start(10);
        vbox->set_margin_end(10);

        auto button_hbox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
        button_hbox->set_margin_top(10);
        button_hbox->set_margin_end(10);

        string iconPath = DirectoryPath::get_data_icon_path();
        auto icon = Gtk::manage(new Gtk::Image(iconPath + "/ICplus.png"));
        select_File_Button.set_image(*icon);
        select_File_Button.set_size_request(100, 40);
        select_File_Button.signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::on_select_image));

        button_hbox->pack_start(*Gtk::manage(new Gtk::Label()), Gtk::PACK_EXPAND_WIDGET);
        button_hbox->pack_start(select_File_Button, Gtk::PACK_SHRINK);

        vbox->pack_start(*button_hbox, Gtk::PACK_SHRINK);

        vbox->pack_start(flowbox, Gtk::PACK_EXPAND_WIDGET);

        scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        scrolled_window.add(*vbox);
    }

    scrolled_window.show_all_children();
}

bool HomeWindow::on_image_click(GdkEventButton *event, const Image &imageInfo)
{
    dialog = new Gtk::Dialog("", true);
    dialog->set_title(localization_Manager.get_key("select_image_info"));
    dialog->set_modal(true);
    dialog->set_default_size(600, 420);

    Gtk::Box *mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12));
    dialog->get_content_area()->pack_start(*mainBox, Gtk::PACK_EXPAND_WIDGET);

    auto pixbuf = Gdk::Pixbuf::create_from_file(imageInfo.getPath());
    auto scaled_pixbuf = pixbuf->scale_simple(480, 270, Gdk::INTERP_BILINEAR);
    auto img = Gtk::manage(new Gtk::Image(scaled_pixbuf));
    auto overlay = Gtk::manage(new Gtk::Overlay());
    overlay->set_margin_bottom(10);
    overlay->add(*img);
    mainBox->pack_start(*overlay, Gtk::PACK_SHRINK);

    Gtk::Frame *infoFrame = Gtk::manage(new Gtk::Frame(""));
    infoFrame->set_shadow_type(Gtk::SHADOW_IN);

    Gtk::Box *infoBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 8));
    infoBox->set_margin_top(10);
    infoBox->set_margin_bottom(10);
    infoBox->set_margin_start(25);
    infoBox->set_margin_end(15);

    auto labelName = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_name") + " : " + imageInfo.getImageName()));
    labelName->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelName, Gtk::PACK_SHRINK);

    auto labelPath = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_path") + " : " + imageInfo.getPath()));
    labelPath->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelPath, Gtk::PACK_SHRINK);

    auto labelType = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_type") + " : " + imageInfo.getType()));
    labelType->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelType, Gtk::PACK_SHRINK);

    auto labelWidth = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_width") + " : " + to_string(imageInfo.getWidth())));
    labelWidth->set_halign(Gtk::ALIGN_START);
    infoBox->pack_start(*labelWidth, Gtk::PACK_SHRINK);

    auto labelHeight = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_height") + " : " + to_string(imageInfo.getHeight())));
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
    walllpaper_btn->set_margin_start(20);
    buttonBox->pack_start(*walllpaper_btn, Gtk::PACK_SHRINK);

    auto open_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICimage_marker.png"));
    auto open_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_open_containing_file")));

    auto contentBox3 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
    contentBox3->pack_start(*open_Btn_Icon, Gtk::PACK_SHRINK);
    contentBox3->pack_start(*open_Btn_Label, Gtk::PACK_SHRINK);

    auto openFile_Btn = Gtk::manage(new Gtk::Button());
    openFile_Btn->add(*contentBox3);
    openFile_Btn->set_margin_start(20);
    buttonBox->pack_start(*openFile_Btn, Gtk::PACK_SHRINK);

    auto delete_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICdelete.png"));
    auto delete_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.get_key("image_delete")));

    auto contentBox2 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
    contentBox2->pack_start(*delete_Btn_Icon, Gtk::PACK_SHRINK);
    contentBox2->pack_start(*delete_Btn_Label, Gtk::PACK_SHRINK);

    auto delete_Btn = Gtk::manage(new Gtk::Button());
    delete_Btn->add(*contentBox2);
    delete_Btn->set_margin_start(20);
    buttonBox->pack_start(*delete_Btn, Gtk::PACK_SHRINK);

    dialog->get_content_area()->pack_start(*buttonBox, Gtk::PACK_SHRINK);

    dialog->show_all();

    selected_Image_Path = imageInfo.getPath();

    walllpaper_btn->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::on_set_wallpaper));
    openFile_Btn->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::open_containing_current_file));
    delete_Btn->signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::delete_image));

    close_Button.set_label(localization_Manager.get_key("close"));
    close_Button.show();
    dialog->add_action_widget(close_Button, Gtk::RESPONSE_CLOSE);

    close_Button.signal_clicked().connect(sigc::mem_fun(*this, &HomeWindow::on_close_dialog));

    dialog->run();

    delete dialog;
    dialog = nullptr;
    return true;
}

void HomeWindow::on_select_image()
{
    Gtk::FileChooserDialog dialog(localization_Manager.get_key("select_image"), Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button(localization_Manager.get_key("cancel"), Gtk::RESPONSE_CANCEL);
    dialog.add_button(localization_Manager.get_key("open"), Gtk::RESPONSE_OK);

    auto filter_image = Gtk::FileFilter::create();
    filter_image->set_name(localization_Manager.get_key("image_files"));
    filter_image->add_mime_type("image/png");
    filter_image->add_mime_type("image/jpeg");
    // filter_image->add_mime_type("image/gif");
    filter_image->add_pattern("*.png");
    filter_image->add_pattern("*.jpg");
    filter_image->add_pattern("*.jpeg");
    // filter_image->add_pattern("*.gif");
    dialog.add_filter(filter_image);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name(localization_Manager.get_key("all_files"));
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK)
    {
        string fileName = dialog.get_filename();
        bool add_image_result = image_Manager.AddImage(fileName);
        if (add_image_result)
        {
            ToastMessage *toast = new ToastMessage(localization_Manager.get_key("success_added_image"));
            toast->show();
            load_images(true);
        }
    }
}

void HomeWindow::on_set_wallpaper()
{
    dialog->hide();
    wallpaper_Manager.set_wallpaper(selected_Image_Path);
    //  ToastMessage *toast = new ToastMessage("success");
    //  toast->show();
}

void HomeWindow::delete_image()
{
    bool delete_image_result = image_Manager.DeleteImage(selected_Image_Path);
    if (delete_image_result)
    {
        dialog->hide();
        ToastMessage *toast = new ToastMessage(localization_Manager.get_key("success_delete_image"));
        toast->show();
        load_images(true);
        return;
    }
    ToastMessage *toast = new ToastMessage(localization_Manager.get_key("error_delete_image"));
    toast->show();
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
}
