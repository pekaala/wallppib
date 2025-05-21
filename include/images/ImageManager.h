#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <vector>
#include <filesystem>
#include <iostream>
#include "Image.h"
#include "../utils/log.h"
#include "../utils/settings.h"

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
        return copy_image_file(selectedFilePath);
    }

    bool DeleteImage(const string &selectedFilePath)
    {
        return delete_image_file(selectedFilePath);
    }

private:
    std::list<Magick::Image> frames;

    void loadImageFile()
    {
        try
        {
            string folderPath = DirectoryPath::get_data_contents_path();

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

    bool copy_image_file(const string &selectedFilePath)
    {
        try
        {
            if (selectedFilePath.empty())
                return false;

            string folderPath = DirectoryPath::get_data_contents_path();

            string fileName = fs::path(selectedFilePath).filename().string();
            string destinationPath = folderPath + "/" + fileName;

            fs::copy_file(selectedFilePath, destinationPath, fs::copy_options::overwrite_existing);

            string resizedPath = folderPath + "/resized_" + fileName;
            if (Settings::read_settings_flag_from_disk())
            {
                try
                {
                    if (is_gif(destinationPath))
                    {
                        thread([destinationPath]()
                               {
                                   string cmd = "convert \"" + destinationPath + "\" -coalesce -resize 1920x1080 \"" + destinationPath + "\"";
                                   int result = std::system(cmd.c_str());

                                   if (result != 0)
                                   {
                                       Log("ImageMagick convert failed.");
                                       return false;
                                   }
                                   else
                                   {
                                       return true;
                                   }
                               })
                            .detach();
                    }
                    else
                    {
                        Magick::Image image(destinationPath);

                        image.resize(Magick::Geometry(1920, 1080));

                        image.write(destinationPath);
                    }
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

    bool delete_image_file(const string &selectedFilePath)
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

    bool is_gif(const string &imagePath)
    {
        return imagePath.size() > 4 && imagePath.substr(imagePath.size() - 4) == ".gif";
    }
};

#endif // IMAGE_MANAGER_H
