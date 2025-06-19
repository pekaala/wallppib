#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>

#include "../directory_path.h"
#include "../log.h"
#include "../settings.h"
#include "i_localization.h"

using json = nlohmann::json;

class LocalizationManager
{
public:
    LocalizationManager();
    ~LocalizationManager();

    void register_localizable_window(ILocalization *window);
    std::string get_key(const std::string &key);
    std::map<std::string, std::string> &get_languages();
    std::string get_selected_language_code(const std::string &code = "");
    void read_data_selected_language_value();
    void change_language(const std::string &code);

private:
    std::map<std::string, std::string> supportedLanguages;
    json langKeyValues;
    std::vector<ILocalization *> localizableWindow;
    std::string find_get_key(const std::string &key);
};

#endif // LOCALIZATION_MANAGER_H
