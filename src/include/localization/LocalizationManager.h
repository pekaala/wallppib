#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

#include <nlohmann/json.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include "../DirectoryPath.h"
#include "../Log.h"
#include "./ILocalization.h"

using namespace std;
using json = nlohmann::json;

const std::string defaultSelectedLanguage = "en-US";
const std::string key_DefaultSelectedLanguage = "selected_language";

class LocalizationManager
{

public:
    LocalizationManager()
    {
        if (supportedLanguages.empty())
        {
            supportedLanguages = {
                {"en-US", "English"},
                {"tr-TR", "Turkish"},
                {"fr-FR", "French"},
                {"de-DE", "German"},
                {"es-ES", "Spanish"}};
        }

        loadSelectedLanguageFromFile();

        if (langKeyValues.size() == 0)
        {
            loadLanguageValueFromFile();
        }
    }

    void RegisterLocalizableView(ILocalization *view)
    {
        localizableViews.push_back(view);
    }

    const std::string GetKey(const std::string &key)
    {
        return getKey(key);
    }

    const std::map<std::string, std::string> &GetLanguages() const
    {
        return supportedLanguages;
    }

    std::string GetSelectedLanguageCode(const std::string &code = "") const
    {
        return currentLanguage;
    }

    void ReadDataSelectedLanguageValue()
    {
        loadSelectedLanguageFromFile();
    }

    void ChangeLanguage(const std::string &code)
    {
        saveLanguageFromFile(code);
    }

private:
    std::string currentLanguage;
    map<string, string> supportedLanguages;
    json langKeyValues;
    std::vector<ILocalization *> localizableViews;

    const std::string getKey(const std::string &key)
    {
        if (langKeyValues.contains(key))
        {
            return langKeyValues[key];
        }
        return "key_not_found";
    }

    void loadSelectedLanguageFromFile()
    {
        try
        {
            std::string filePath = DirectoryPath::GetDataLocalizationPath();
            std::string combinePath = filePath + "/data.json";

            std::ifstream file(combinePath);
            if (!file.is_open())
            {
                Log("Error: Could not open the file! readDataSelectedLanguageValue");
                return;
            }
            json jsonData;
            file >> jsonData;
            file.close();

            if (jsonData.contains(key_DefaultSelectedLanguage))
            {
                std::string selectedLang = jsonData[key_DefaultSelectedLanguage].get<std::string>();
                currentLanguage = selectedLang;
                return;
            }
            else
            {
                Log("Error: " + key_DefaultSelectedLanguage + " key not found in JSON! and Set Default language value");
                currentLanguage = defaultSelectedLanguage;
            }
        }
        catch (const std::exception &e)
        {
            Log(e.what());
        }
    }

    void loadLanguageValueFromFile()
    {
        try
        {
            if (currentLanguage.empty())
                return;

            std::string filePath = DirectoryPath::GetDataLocalizationPath();
            std::string combinePath = filePath + "/" + currentLanguage + ".json";

            std::ifstream file(combinePath);
            if (!file.is_open())
            {
                Log("Error: Could not open the file! loadLanguageValues");
                return;
            }
            file >> langKeyValues;
            file.close();
            Log(currentLanguage + std::to_string(langKeyValues.size()));
        }
        catch (const std::exception &e)
        {
            Log(e.what());
        }
    }

    void saveLanguageFromFile(const std::string &code)
    {
        try
        {
            std::string filePath = DirectoryPath::GetDataLocalizationPath();
            std::string combinePath = filePath + "/data.json";

            json jsonData;
            jsonData["selected_language"] = code;
            jsonData["default_language"] = defaultSelectedLanguage;

            std::ofstream outFile(combinePath);
            if (outFile.is_open())
            {
                outFile << jsonData.dump(4);
                outFile.close();
                Log("The file has been saved successfully.");

                currentLanguage = code;
                loadLanguageValueFromFile();

                for (auto *view : localizableViews)
                {
                    if (view)
                        view->listener_update_ui();
                }
            }
            else
            {
                Log("Warning: Cannot proceed, the file is not active.");
            }
        }
        catch (const std::exception &e)
        {
            Log(e.what());
        }
    }
};

#endif // LOCALIZATION_MANAGER_H
