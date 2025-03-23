#ifndef EXPLORE_H
#define EXPLORE_H

#include <iostream>
#include <vector>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/grid.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm.h>
#include "Windowsize.h"
#include <gtkmm/eventbox.h>
#include "Images.h"
#include "ImageInfo.h"
#include <gtkmm/messagedialog.h>
#include <gtkmm/button.h>
#include "WallpaperEngine.h"

using namespace std;

class ExploreWindow
{
public:
    ExploreWindow()
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
                sigc::bind(sigc::mem_fun(*this, &ExploreWindow::on_image_click), imageInfo));

            int row = i / 2;
            int col = i % 2;

            m_ImageGrid.attach(*eventBox, col, row, 1, 1);

            m_EventBoxes.push_back(eventBox);
            m_Images.push_back(img);
        }

        m_Layout.put(m_ScrolledWindow, 0, 100);

        m_ImageNameLabel.set_margin_end(330);
        m_ImageNameLabel.set_margin_start(1320);
        m_ImageNameLabel.set_text("No image selected.");
        m_Layout.put(m_ImageNameLabel, 50, 500);
    }

    Gtk::Layout &get_layout()
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
    windowsize win_Size;
    Images imgs;
    WallpaperEngine wallpaper_Engine;

    bool on_image_click(GdkEventButton *event, const ImageInfo &imageInfo)
    {
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(imageInfo.getPath());
        Glib::RefPtr<Gdk::Pixbuf> scaled_pixbuf = pixbuf->scale_simple(480, 270, Gdk::INTERP_BILINEAR);

        Gtk::Image *img = Gtk::manage(new Gtk::Image(scaled_pixbuf));

        img->set_margin_start(1320);
        img->set_margin_top(45);

        Gtk::Box *box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));
        box->pack_start(*img, Gtk::PACK_SHRINK);

        m_Layout.put(*box, 50, 50);

        selected_Image_Path = imageInfo.getPath();

        std::string imageTextInfo =
            "ImageName: " + imageInfo.getImageName() + "\n" +
            "Path: " + imageInfo.getPath() + "\n" +
            "Type: " + imageInfo.getType() + "\n" +
            "Width: " + std::to_string(imageInfo.getWidth()) + "\n" +
            "Height: " + std::to_string(imageInfo.getHeight());

        m_ImageNameLabel.set_text(imageTextInfo);
        m_Layout.put(m_ImageNameLabel, 50, 500);

        set_WallpButton.set_label("Set Wallpaper");
        set_WallpButton.set_margin_end(1);
        set_WallpButton.set_margin_top(95);
        set_WallpButton.set_margin_start(1320);

        set_WallpButton.signal_clicked().connect(sigc::mem_fun(*this, &ExploreWindow::on_set_wallpaper));

        m_Layout.put(set_WallpButton, 50, 500);

        m_Layout.show_all();
        return true;
    }

    void on_set_wallpaper()
    {
        wallpaper_Engine.setWallpaper(selected_Image_Path);
    }
};

#endif // EXPLORE_H
