#ifndef SETTINGS_H
#define SETTINGS_H

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include "../DirectoryPath.h"
#include "../Log.h"
#include <filesystem>

using json = nlohmann::json;
using namespace std;

class Settings
{
public:
    static void OnSettingsChanged(bool type)
    {
        saveSettingsFromFile(type);
    }

    static bool GetSettingsValue()
    {
        return loadSettingsValueFromFile();
    }

private:
    static void saveSettingsFromFile(bool type)
    {
        try
        {
            string filePath = DirectoryPath::GetDataLocalizationPath();
            string combinePath = filePath + "/data.json";

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

            jsonData["automatically_resize_image"] = type;

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

    static bool loadSettingsValueFromFile()
    {
        try
        {
            string filePath = DirectoryPath::GetDataLocalizationPath();
            string combinePath = filePath + "/data.json";

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
};

#endif // SETTINGS_H
