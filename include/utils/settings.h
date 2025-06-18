#ifndef SETTINGS_H
#define SETTINGS_H

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "./directory_path.h"
#include "./log.h"

using json = nlohmann::json;
using namespace std;

class Settings
{
public:
    static void handle_settings_toggle(bool enabled);
    static bool get_current_settings_config_value();
    static void load_active_language_from_disk();
    static void read_language_code_from_config();
    static void write_language_code_to_config(const string &code);
    static bool read_settings_config();
    static string current_language;
    static json lang_key_values;

private:
    static string default_selected_language;
    static string key_default_selected_language;
    static bool automatically_resize_image;
    static void write_settings_flag_to_disk(bool flag);
};

#endif // SETTINGS_H
