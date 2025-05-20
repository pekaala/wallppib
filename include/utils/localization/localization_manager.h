#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

#include <nlohmann/json.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include "../directory_path.h"
#include "../log.h"
#include "../settings.h"
#include "i_localization.h"

using namespace std;
using json = nlohmann::json;

class LocalizationManager
{
public:
    LocalizationManager();
    ~LocalizationManager();

    void register_localizable_window(ILocalization *window);
    string get_key(const string &key);
    map<string, string> &get_languages();
    string get_selected_language_code(const string &code = "");
    void read_data_selected_language_value();
    void change_language(const string &code);

private:
    map<string, string> supportedLanguages;
    json langKeyValues;
    vector<ILocalization *> localizableWindow;
    string find_get_key(const string &key);
};
#endif // LOCALIZATION_MANAGER_H