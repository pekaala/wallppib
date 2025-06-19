#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <vector>
#include <filesystem>
#include <iostream>
#include <future>
#include <gdkmm/pixbuf.h>
#include <glibmm/exception.h>

#include "../utils/log.h"
#include "../utils/settings.h"
#include "../domain/image_info.h"

class ImageManager
{
public:
    std::vector<Image> Images;
    std::future<void> get_all_images(const std::string &filterType);
    std::future<bool> add_image_async(const std::string &selectedFilePath);
    bool delete_image(const std::string &selectedFilePath);

protected:
    void load_image_file(const std::string &filterType);
    bool copy_image_file(const std::string &selectedFilePath);
    bool copy_image_linux(const std::string &selectedFilePath);
    bool copy_image_windows(const std::string &selectedFilePath);
    bool copy_image_macos(const std::string &selectedFilePath);
    bool delete_image_file(const std::string &selectedFilePath);
    bool delete_image_linux(const std::string &selectedFilePath);
    bool delete_image_windows(const std::string &selectedFilePath);
    bool delete_image_macos(const std::string &selectedFilePath);
    bool result_type(int res);

private:
    std::list<Magick::Image> frames;
};

#endif // IMAGE_MANAGER_H
