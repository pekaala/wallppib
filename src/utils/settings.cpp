#include "../../include/utils/settings.h"

string Settings::default_selected_language = "en-US";
string Settings::key_default_selected_language = "selected_language";
bool Settings::automatically_resize_image = false;

string Settings::current_language = default_selected_language;
json Settings::lang_key_values = json::object();

void Settings::handle_settings_toggle(bool enabled)
{
    Settings::write_settings_flag_to_disk(enabled);
}

bool Settings::get_current_settings_config_value()
{
    return Settings::read_settings_config();
}

void Settings::load_active_language_from_disk()
{
    try
    {
        string filePath = DirectoryPath::get_data_localization_path();
        string combinePath = filePath + "/config.json";

        ifstream file(combinePath);
        if (!file.is_open())
        {
            Log("Error: Could not open the file! load_active_language_from_disk");
            Log("Error:" + combinePath);
            return;
        }
        json jsonData;
        file >> jsonData;
        file.close();

        if (jsonData.contains(key_default_selected_language))
        {
            string selectedLang = jsonData[key_default_selected_language].get<string>();
            current_language = selectedLang;
            return;
        }
        else
        {
            Log("Error: " + key_default_selected_language + " key not found in JSON! and Set Default language value");
            current_language = default_selected_language;
        }
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

void Settings::read_language_code_from_config()
{
    try
    {
        if (current_language.empty())
            return;

        string filePath = DirectoryPath::get_data_localization_path();
        string combinePath = filePath + "/" + current_language + ".json";

        ifstream file(combinePath);
        if (!file.is_open())
        {
            Log("Error: Could not open the file! read_language_code_from_config");
            Log("Error:" + combinePath);
            return;
        }
        file >> lang_key_values;
        file.close();
        Log(current_language + to_string(lang_key_values.size()));
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

void Settings::write_language_code_to_config(const string &code)
{
    try
    {
        string filePath = DirectoryPath::get_data_localization_path();
        string combinePath = filePath + "/config.json";

        Log(combinePath.c_str());

        json jsonData;

        jsonData["selected_language"] = code;
        jsonData["default_language"] = default_selected_language;
        jsonData["automatically_resize_image"] = automatically_resize_image;

        ofstream outFile(combinePath);
        if (outFile.is_open())
        {
            outFile << jsonData.dump(4);
            outFile.close();
            Log("The file has been saved successfully.");

            current_language = code;
            Settings::read_language_code_from_config();
        }
        else
        {
            Log("Warning: Cannot proceed, the file is not active.");
        }
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

void Settings::write_settings_flag_to_disk(bool flag)
{
    try
    {
        string filePath = DirectoryPath::get_data_localization_path();
        string combinePath = filePath + "/config.json";

        json jsonData;
        ifstream inFile(combinePath);
        if (inFile.is_open())
        {
            inFile >> jsonData;
            inFile.close();
        }
        else
        {
            Log("Cannot open file for reading.");
            return;
        }

        automatically_resize_image = flag;
        jsonData["automatically_resize_image"] = flag;

        ofstream outFile(combinePath);
        if (outFile.is_open())
        {
            outFile << jsonData.dump(4);
            outFile.close();
            Log("The file has been saved successfully.");
        }
        else
        {
            Log("Cannot open file for writing.");
        }
    }
    catch (const exception &e)
    {
        Log(e.what());
    }
}

bool Settings::read_settings_config()
{
    try
    {
        string filePath = DirectoryPath::get_data_localization_path();
        string combinePath = filePath + "/config.json";

        json jsonData;
        ifstream inFile(combinePath);
        if (inFile.is_open())
        {
            inFile >> jsonData;
            inFile.close();
            return jsonData["automatically_resize_image"].get<bool>();
        }
        else
        {
            Log("Warning: Cannot proceed, the file is not active.");
            return false;
        }
    }
    catch (const exception &e)
    {
        Log(e.what());
        return false;
    }
}
