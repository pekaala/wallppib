#ifndef DIRECTORY_PATH_H
#define DIRECTORY_PATH_H

#include <filesystem>
#include <string>
#include <unistd.h>

namespace fs = std::filesystem;

class DirectoryPath
{
public:
    static void initialize_user_config();
    static void create_desktop_entry_if_missing(const std::string &path);
    static std::string get_data_contents_path();
    static std::string get_data_icon_path();
    static std::string get_data_localization_path();
    static std::string get_style_path();
    static void open_containing_folder(const std::string &file_path);

private:
    static std::string get_folder_path(const std::string &file_path);
    static std::string get_desktop_dir();
};

#endif // DIRECTORY_PATH_H
