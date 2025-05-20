#include "../../../include/utils/localization/localization_manager.h"

LocalizationManager::LocalizationManager()
{
    if (supportedLanguages.empty())
    {
        supportedLanguages =
            {
                {"en-US", "English"},
                {"tr-TR", "Turkish"},
                {"fr-FR", "French"},
                {"de-DE", "German"},
                {"es-ES", "Spanish"}};
    }
    Settings::load_active_language_from_disk();

    if (Settings::lang_key_values.size() == 0)
    {
        Settings::read_language_code_from_config();
    }
}

LocalizationManager::~LocalizationManager()
{
}

void LocalizationManager::register_localizable_window(ILocalization *window)
{
    localizableWindow.push_back(window);
}

string LocalizationManager::get_key(const string &key)
{
    return LocalizationManager::find_get_key(key);
}

map<string, string> &LocalizationManager::get_languages()
{
    return supportedLanguages;
}

string LocalizationManager::get_selected_language_code(const string &code)
{
    return Settings::current_language;
}

void LocalizationManager::read_data_selected_language_value()
{
    Settings::read_language_code_from_config();
}

void LocalizationManager::change_language(const string &code)
{
    Settings::write_language_code_to_config(code);

    for (auto *window : localizableWindow)
    {
        if (window)
            window->listener_update_ui();
    }
}

string LocalizationManager::find_get_key(const string &key)
{
    if (Settings::lang_key_values.contains(key))
    {
        return Settings::lang_key_values[key];
    }
    return "key_not_found";
}
