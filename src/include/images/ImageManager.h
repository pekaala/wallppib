#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <vector>
#include <filesystem>
#include <iostream>
#include "../Log.h"
#include "Image.h"
#include "../settings/Settings.h"

using namespace std;

class ImageManager
{

public:
    vector<Image> Images;

    void GetImages()
    {
        Images.clear();

        loadImageFile();
    }

    bool AddImage(const string &selectedFilePath)
    {
        return copyImageFile(selectedFilePath);
    }

    bool DeleteImage(const string &selectedFilePath)
    {
        return deleteImageFile(selectedFilePath);
    }

private:
    void loadImageFile()
    {
        try
        {
            string folderPath = DirectoryPath::GetDataContentsPath();

            if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
            {
                Log("Error: Invalid directory path! getImages");
                return;
            }

            for (const auto &entry : fs::directory_iterator(folderPath))
            {
                if (entry.is_regular_file())
                {
                    string fileName = entry.path().filename().string();
                    string fullPath = entry.path().string();
                    string extension = entry.path().extension().string();

                    if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".gif")
                    {
                        cv::Mat image = cv::imread(fullPath);

                        int width = 0;
                        int height = 0;
                        if (!image.empty())
                        {
                            width = image.cols;
                            height = image.rows;
                        }

                        Images.emplace_back(fileName, fullPath, width, height);
                    }
                }
            }
        }
        catch (const exception &e)
        {
            Log(e.what());
        }
    }

    bool copyImageFile(const string &selectedFilePath)
    {
        try
        {
            if (selectedFilePath.empty())
                return false;

            string folderPath = DirectoryPath::GetDataContentsPath();

            string fileName = fs::path(selectedFilePath).filename().string();
            string destinationPath = folderPath + "/" + fileName;

            fs::copy_file(selectedFilePath, destinationPath, fs::copy_options::overwrite_existing);

            if (Settings::GetSettingsValue())
            {
                try
                {
                    Magick::Image image(destinationPath);

                    image.resize(Magick::Geometry(1920, 1080));

                    image.write(destinationPath);
                }
                catch (const exception &e)
                {
                    Log(e.what());
                    return false;
                }
            }

            return true;
        }
        catch (const exception &e)
        {
            Log(e.what());
            return false;
        }
    }

    bool deleteImageFile(const string &selectedFilePath)
    {
        try
        {
            if (selectedFilePath.empty())
                return false;

            fs::remove(selectedFilePath);
            return true;
        }
        catch (const exception &e)
        {
            Log(e.what());
            return false;
        }
    }
};

#endif // IMAGE_MANAGER_H
