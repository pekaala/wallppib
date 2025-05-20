
#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/separator.h>
#include <gtkmm/image.h>
#include <gtkmm/dialog.h>
#include "../utils/directory_path.h"
#include "../utils/log.h"
#include "../utils/localization/i_localization.h"
#include "../utils/localization/localization_manager.h"
#include "../common/window_size.h"

class AboutWindow : public ILocalization
{

public:
    AboutWindow(LocalizationManager &locManager);
    ~AboutWindow();

    void show_about_dialog(Gtk::Window &parent);

private:
    LocalizationManager &localization_Manager;
    WindowSize win_Size;

protected:
    void on_donate_clicked();
    void on_feedback_clicked();
    void on_source_code_clicked();
    void listener_update_ui() override;
};

#endif // ABOUT_WINDOW_H