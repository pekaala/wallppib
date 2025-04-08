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
#include "../Images.h"
#include "../ImageInfo.h"
#include "../WallpaperEngine.h"
#include "../Windowsize.h"
#include "../localization/LocalizationManager.h"
#include "../localization/ILocalization.h"

class ExploreView : public ILocalization
{
public:
    ExploreView(LocalizationManager &locManager)
        : localization_Manager(locManager)
    {
        m_Layout.set_size(win_Size.W, win_Size.H);

        /* m_ExploreLabel.set_text("This is the Explore");
        m_Layout.put(m_ExploreLabel, 50, 50); */

        m_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        m_ScrolledWindow.add(m_ImageGrid);

        m_ScrolledWindow.set_size_request(win_Size.W, win_Size.H * 1.5);
        m_ScrolledWindow.set_vexpand(true);
        m_ScrolledWindow.set_hexpand(true);

        m_ImageGrid.set_row_spacing(13);
        m_ImageGrid.set_column_spacing(13);

        for (int i = 0; i < imgs.images.size(); ++i)
        {
            const ImageInfo &imageInfo = imgs.images[i];

            const std::string path = imageInfo.getPath();

            Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(path);
            Glib::RefPtr<Gdk::Pixbuf> scaled_pixbuf = pixbuf->scale_simple(640, 360, Gdk::INTERP_BILINEAR);

            Gtk::Image *img = Gtk::manage(new Gtk::Image(scaled_pixbuf));
            img->set_halign(Gtk::ALIGN_CENTER);
            img->set_valign(Gtk::ALIGN_CENTER);
            img->set_size_request(640, 360);

            // m_Images.push_back(img);

            Gtk::EventBox *eventBox = Gtk::manage(new Gtk::EventBox());
            eventBox->add(*img);

            eventBox->signal_button_press_event().connect(
                sigc::bind(sigc::mem_fun(*this, &ExploreView::on_image_click), imageInfo));

            int row = i / 2;
            int col = i % 2;

            m_ImageGrid.attach(*eventBox, col, row, 1, 1);

            m_EventBoxes.push_back(eventBox);
            m_Images.push_back(img);
        }

        m_Layout.put(m_ScrolledWindow, 0, 100);

        m_ImageNameLabel.set_margin_end(330);
        m_ImageNameLabel.set_margin_start(1320);
        m_ImageNameLabel.set_label(localization_Manager.GetKey("no_selected_image"));
        m_Layout.put(m_ImageNameLabel, 50, 500);
    }

    Gtk::Layout &Get_Layout_Explore()
    {
        return m_Layout;
    }

private:
    Gtk::Layout m_Layout;
    Gtk::Grid m_ImageGrid;
    std::vector<Gtk::Image *> m_Images;
    Gtk::Label m_ExploreLabel;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::Label m_ImageNameLabel;
    std::vector<Gtk::EventBox *> m_EventBoxes;
    Gtk::Button set_WallpButton;
    std::string selected_Image_Path;
    Windowsize win_Size;
    Images imgs;
    WallpaperEngine wallpaper_Engine;
    LocalizationManager &localization_Manager;
    bool m_IsSelected_Image = false;

    bool on_image_click(GdkEventButton *event, const ImageInfo &imageInfo)
    {
        m_IsSelected_Image = true;
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(imageInfo.getPath());
        Glib::RefPtr<Gdk::Pixbuf> scaled_pixbuf = pixbuf->scale_simple(480, 270, Gdk::INTERP_BILINEAR);

        Gtk::Image *img = Gtk::manage(new Gtk::Image(scaled_pixbuf));

        img->set_margin_start(1320);
        img->set_margin_top(45);

        Gtk::Box *box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
        box->pack_start(*img, Gtk::PACK_SHRINK);

        m_Layout.put(*box, 50, 50);

        selected_Image_Path = imageInfo.getPath();

        //  NOTE: Not localized on purpose – this line will be removed in the future
        std::string imageTextInfo =
            "ImageName: " + imageInfo.getImageName() + "\n" +
            "Path: " + imageInfo.getPath() + "\n" +
            "Type: " + imageInfo.getType() + "\n" +
            "Width: " + std::to_string(imageInfo.getWidth()) + "\n" +
            "Height: " + std::to_string(imageInfo.getHeight());

        m_ImageNameLabel.set_text(imageTextInfo);
        m_Layout.put(m_ImageNameLabel, 50, 500);

        set_WallpButton.set_label(localization_Manager.GetKey("set_wallpaper"));
        set_WallpButton.set_margin_end(1);
        set_WallpButton.set_margin_top(95);
        set_WallpButton.set_margin_start(1320);

        set_WallpButton.signal_clicked().connect(sigc::mem_fun(*this, &ExploreView::on_set_wallpaper));

        m_Layout.put(set_WallpButton, 50, 500);

        m_Layout.show_all();
        return true;
    }

    void on_set_wallpaper()
    {
        wallpaper_Engine.setWallpaper(selected_Image_Path);
    }

    void listener_update_ui() override
    {
        if (!m_IsSelected_Image)
        {
            m_ImageNameLabel.set_label(localization_Manager.GetKey("no_selected_image"));
        }

        set_WallpButton.set_label(localization_Manager.GetKey("set_wallpaper"));
    }
};

#endif // EXPLORE_VIEW_H
