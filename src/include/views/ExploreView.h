#ifndef EXPLORE_VIEW_H
#define EXPLORE_VIEW_H

#include <iostream>
#include <vector>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/grid.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/button.h>
#include "../wallpaper/WallpaperManager.h"
#include "../localization/LocalizationManager.h"
#include "../localization/ILocalization.h"
#include "../images/Image.h"
#include "../images/ImageManager.h"
#include "../Log.h"
#include "ToastMessageView.h"
#include "../common/WindowSize.h"
#include "../DirectoryPath.h"

using namespace std;

class ExploreView : public ILocalization
{
public:
    ExploreView(LocalizationManager &locManager) : localization_Manager(locManager)
    {
        m_Layout.set_size(win_Size.W, win_Size.H);

        string iconPath = DirectoryPath::GetDataIconPath();

        auto icon = Gtk::manage(new Gtk::Image(iconPath + "/ICplus.png"));
        select_File_Button.set_image(*icon);
        select_File_Button.set_margin_start(10);
        select_File_Button.signal_clicked().connect(sigc::mem_fun(*this, &ExploreView::on_select_image));
        m_Layout.put(select_File_Button, 15, 30);

        m_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        m_ScrolledWindow.add(m_ImageGrid);

        m_ScrolledWindow.set_size_request(win_Size.W, 1000);
        /* m_ScrolledWindow.set_vexpand(true);
           m_ScrolledWindow.set_hexpand(true);
        */

        m_ImageGrid.set_row_spacing(15);
        m_ImageGrid.set_column_spacing(25);

        m_Layout.put(m_ScrolledWindow, 25, 80);

        load_images();
    }

    Gtk::Layout &Get_Layout_Explore()
    {
        return m_Layout;
    }

private:
    Gtk::Layout m_Layout;
    Gtk::Grid m_ImageGrid;
    vector<Gtk::Image *> m_Images;
    Gtk::Label m_ExploreLabel;
    Gtk::ScrolledWindow m_ScrolledWindow;
    vector<Gtk::EventBox *> m_EventBoxes;
    string selected_Image_Path;
    Gtk::Button select_File_Button;
    Gtk::Dialog *dialog = nullptr;
    Gtk::Button close_Button;
    WindowSize win_Size;
    WallpaperManager wallpaper_Manager;
    LocalizationManager &localization_Manager;
    ImageManager image_Manager;

    void load_images()
    {
        m_ImageGrid.foreach ([&](Gtk::Widget &child)
                             { m_ImageGrid.remove(child); });

        m_Images.clear();
        m_EventBoxes.clear();

        image_Manager.GetImages();

        for (int i = 0; i < image_Manager.Images.size(); ++i)
        {
            const Image &imageInfo = image_Manager.Images[i];

            const string path = imageInfo.getPath();

            Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(path);
            Glib::RefPtr<Gdk::Pixbuf> scaled_pixbuf = pixbuf->scale_simple(540, 304, Gdk::INTERP_BILINEAR);

            Gtk::Image *img = Gtk::manage(new Gtk::Image(scaled_pixbuf));
            img->set_halign(Gtk::ALIGN_CENTER);
            img->set_valign(Gtk::ALIGN_CENTER);
            img->set_size_request(540, 304);

            Gtk::EventBox *eventBox = Gtk::manage(new Gtk::EventBox());
            eventBox->add(*img);

            eventBox->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this, &ExploreView::on_image_click), imageInfo));

            int row = i / 3;
            int col = i % 3;

            m_ImageGrid.attach(*eventBox, col, row, 1, 1);

            m_EventBoxes.push_back(eventBox);
            m_Images.push_back(img);
        }

        m_ImageGrid.show_all();
    }

    bool on_image_click(GdkEventButton *event, const Image &imageInfo)
    {
        dialog = new Gtk::Dialog("", true);
        dialog->set_title(localization_Manager.GetKey("select_image_info"));
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

        auto labelName = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_name") + " : " + imageInfo.getImageName()));
        labelName->set_halign(Gtk::ALIGN_START);
        infoBox->pack_start(*labelName, Gtk::PACK_SHRINK);

        auto labelPath = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_path") + " : " + imageInfo.getPath()));
        labelPath->set_halign(Gtk::ALIGN_START);
        infoBox->pack_start(*labelPath, Gtk::PACK_SHRINK);

        auto labelType = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_type") + " : " + imageInfo.getType()));
        labelType->set_halign(Gtk::ALIGN_START);
        infoBox->pack_start(*labelType, Gtk::PACK_SHRINK);

        auto labelWidth = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_width") + " : " + to_string(imageInfo.getWidth())));
        labelWidth->set_halign(Gtk::ALIGN_START);
        infoBox->pack_start(*labelWidth, Gtk::PACK_SHRINK);

        auto labelHeight = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_height") + " : " + to_string(imageInfo.getHeight())));
        labelHeight->set_halign(Gtk::ALIGN_START);
        infoBox->pack_start(*labelHeight, Gtk::PACK_SHRINK);

        infoFrame->add(*infoBox);
        mainBox->pack_start(*infoFrame, Gtk::PACK_SHRINK);

        string iconPath = DirectoryPath::GetDataIconPath();

        auto buttonBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 4));

        auto wallpaper_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICwallpaper.png"));
        auto walllpaper_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("set_wallpaper")));

        auto contentBox1 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
        contentBox1->pack_start(*wallpaper_Btn_Icon, Gtk::PACK_SHRINK);
        contentBox1->pack_start(*walllpaper_Btn_Label, Gtk::PACK_SHRINK);

        auto walllpaper_btn = Gtk::manage(new Gtk::Button());
        walllpaper_btn->add(*contentBox1);
        walllpaper_btn->set_margin_start(20);
        buttonBox->pack_start(*walllpaper_btn, Gtk::PACK_SHRINK);

        auto open_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICimage_marker.png"));
        auto open_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_open_containing_file")));

        auto contentBox3 = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 1));
        contentBox3->pack_start(*open_Btn_Icon, Gtk::PACK_SHRINK);
        contentBox3->pack_start(*open_Btn_Label, Gtk::PACK_SHRINK);

        auto openFile_Btn = Gtk::manage(new Gtk::Button());
        openFile_Btn->add(*contentBox3);
        openFile_Btn->set_margin_start(20);
        buttonBox->pack_start(*openFile_Btn, Gtk::PACK_SHRINK);

        auto delete_Btn_Icon = Gtk::manage(new Gtk::Image(iconPath + "/ICdelete.png"));
        auto delete_Btn_Label = Gtk::manage(new Gtk::Label(localization_Manager.GetKey("image_delete")));

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

        walllpaper_btn->signal_clicked().connect(sigc::mem_fun(*this, &ExploreView::on_set_wallpaper));
        openFile_Btn->signal_clicked().connect(sigc::mem_fun(*this, &ExploreView::open_containing_current_file));
        delete_Btn->signal_clicked().connect(sigc::mem_fun(*this, &ExploreView::delete_image));

        close_Button.set_label(localization_Manager.GetKey("close"));
        close_Button.show();
        dialog->add_action_widget(close_Button, Gtk::RESPONSE_CLOSE);

        close_Button.signal_clicked().connect(sigc::mem_fun(*this, &ExploreView::on_close_dialog));

        dialog->run();

        delete dialog;
        dialog = nullptr;

        return true;
    }

    void on_select_image()
    {
        Gtk::FileChooserDialog dialog(localization_Manager.GetKey("select_image"), Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.add_button(localization_Manager.GetKey("cancel"), Gtk::RESPONSE_CANCEL);
        dialog.add_button(localization_Manager.GetKey("open"), Gtk::RESPONSE_OK);

        auto filter_image = Gtk::FileFilter::create();
        filter_image->set_name(localization_Manager.GetKey("image_files"));
        filter_image->add_mime_type("image/png");
        filter_image->add_mime_type("image/jpeg");
        // filter_image->add_mime_type("image/gif");
        filter_image->add_pattern("*.png");
        filter_image->add_pattern("*.jpg");
        filter_image->add_pattern("*.jpeg");
        // filter_image->add_pattern("*.gif");
        dialog.add_filter(filter_image);

        auto filter_any = Gtk::FileFilter::create();
        filter_any->set_name(localization_Manager.GetKey("all_files"));
        filter_any->add_pattern("*");
        dialog.add_filter(filter_any);

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK)
        {
            string fileName = dialog.get_filename();
            bool add_image_result = image_Manager.AddImage(fileName);
            if (add_image_result)
            {
                ToastMessageView *toast = new ToastMessageView(localization_Manager.GetKey("success_added_image"));
                toast->show();
                load_images();
            }
        }
    }

    void on_set_wallpaper()
    {
        dialog->hide();
        wallpaper_Manager.SetWallpaper(selected_Image_Path);
        /* ToastMessageView *toast = new ToastMessageView("success");
        toast->show(); */
    }

    void delete_image()
    {
        bool delete_image_result = image_Manager.DeleteImage(selected_Image_Path);
        if (delete_image_result)
        {
            dialog->hide();
            ToastMessageView *toast = new ToastMessageView(localization_Manager.GetKey("success_delete_image"));
            toast->show();
            load_images();
            return;
        }
        ToastMessageView *toast = new ToastMessageView(localization_Manager.GetKey("error_delete_image"));
        toast->show();
    }

    void open_containing_current_file()
    {
        DirectoryPath::OpenContainingFolder(selected_Image_Path);
    }

    void on_close_dialog()
    {
        dialog->hide();
    }

    void listener_update_ui() override
    {
    }
};

#endif // EXPLORE_VIEW_H
