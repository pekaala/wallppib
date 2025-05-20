#ifndef DIRECTORY_PATH_H
#define DIRECTORY_PATH_H

#include <string>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

class DirectoryPath
{

public:
    static string get_data_contents_path();
    static string get_data_icon_path();
    static string get_data_localization_path();
    static void open_containing_folder(const string &file_path);

private:
    static string get_folder_path(const string &file_path);
};

#endif // DIRECTORY_PATH_H
