#include "../../include/services/image_manager.h"

/*PUBLIC*/
future<void> ImageManager::get_all_images(const string &filterType)
{
    return async(launch::async, [=]()
                 { load_image_file(filterType); });
}

future<bool> ImageManager::add_image_async(const string &selectedFilePath)
{
    return async(launch::async, [=]()
                 { return copy_image_file(selectedFilePath); });
}

bool ImageManager::delete_image(const string &selectedFilePath)
{
    return delete_image_file(selectedFilePath);
}

/*PROTECTED*/
void ImageManager::load_image_file(const string &filterType)
{
    try
    {
        Images.clear();
        string folderPath = DirectoryPath::get_data_contents_path();

        if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
        {
            Log("Error: Invalid directory path! getImages");
            return;
        }

        for (const auto &entry : fs::directory_iterator(folderPath))
        {
            if (!entry.is_regular_file())
                continue;

            string fileName = entry.path().filename().string();
            string fullPath = entry.path().string();
            string extension = entry.path().extension().string();

            transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            bool isGif = extension == ".gif";
            bool isJpg = extension == ".jpg" || extension == ".jpeg";
            bool isPng = extension == ".png";

            if (
                (filterType == "jpg" && isJpg) ||
                (filterType == "png" && isPng) ||
                (filterType == "gif" && isGif) ||
                (filterType == "all" && (isJpg || isPng || isGif)))
            {
                if (isGif)
                {
                    auto pixbuf = Gdk::Pixbuf::create_from_file(fullPath);
                    if (pixbuf)
                    {
                        int width = pixbuf->get_width();
                        int height = pixbuf->get_height();
                        Images.emplace_back(fileName, fullPath, width, height);
                    }
                }
                else
                {
                    cv::Mat image = cv::imread(fullPath, cv::IMREAD_UNCHANGED);

                    if (!image.empty() &&
                        image.cols > 0 &&
                        image.rows > 0 &&
                        image.total() > 0 &&
                        image.data != nullptr)
                    {
                        int width = image.cols;
                        int height = image.rows;
                        Images.emplace_back(fileName, fullPath, width, height);
                    }
                    else
                    {
                        Log("Could not read image: " + fullPath);
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        Log(string("Exception in load_image_file: ") + e.what());
    }
}

bool ImageManager::copy_image_file(const string &selectedFilePath)
{
    try
    {
        if (selectedFilePath.empty())
            return false;

#if defined(__linux__)
        return copy_image_linux(selectedFilePath);
#elif defined(_WIN32)
        return copy_image_windows(selectedFilePath);
#elif defined(__APPLE__)
        return copy_image_macos(selectedFilePath);
#else
        Log("Unsupported platform");
        return false;
#endif
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::copy_image_linux(const string &selectedFilePath)
{
    string cmd = "";

    try
    {
        string folderPath = DirectoryPath::get_data_contents_path();

        string fileName = fs::path(selectedFilePath).filename().string();
        string destinationPath = folderPath + "/" + fileName;

        if (!Settings::get_current_settings_config_value())
        {
            cmd = "cp \"" + selectedFilePath + "\" \"" + folderPath + "\"";
            int result = system(cmd.c_str());
            return result_type(result);
        }
        else
        {
            cmd = "convert \"" + selectedFilePath + "\" -coalesce -resize 1920x1080 \"" + destinationPath + "\"";
            int result = system(cmd.c_str());
            return result_type(result);
        }
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
    return false;
}

bool ImageManager::copy_image_windows(const string &selectedFilePath)
{
    string cmd = "";

    try
    {
        return false;
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::copy_image_macos(const string &selectedFilePath)
{
    string cmd = "";

    try
    {
        return false;
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::delete_image_file(const string &selectedFilePath)
{
    try
    {
        if (selectedFilePath.empty())
            return false;

#if defined(__linux__)
        return delete_image_linux(selectedFilePath);
#elif defined(_WIN32)
        return delete_image_windows(selectedFilePath);
#elif defined(__APPLE__)
        return delete_image_macos(selectedFilePath);
#else
        Log("Unsupported platform");
        return false;
#endif
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::delete_image_linux(const string &selectedFilePath)
{
    try
    {
        if (selectedFilePath.empty())
            return false;

        string cmd = "rm -f \"" + selectedFilePath + "\"";
        int result = system(cmd.c_str());
        return result_type(result);
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::delete_image_windows(const string &selectedFilePath)
{
    try
    {
        if (selectedFilePath.empty())
            return false;

        fs::remove(selectedFilePath);
        return true;
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::delete_image_macos(const string &selectedFilePath)
{
    try
    {
        if (selectedFilePath.empty())
            return false;

        fs::remove(selectedFilePath);
        return true;
    }
    catch (const std::exception &e)
    {
        Log(e.what());
        return false;
    }
}

bool ImageManager::result_type(int res)
{
    return res != 0 ? false : true;
}
