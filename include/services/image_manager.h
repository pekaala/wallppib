#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <vector>
#include <filesystem>
#include <iostream>
#include <future>
#include "../utils/log.h"
#include "../utils/settings.h"
#include "../domain/image_info.h"
#include <gdkmm/pixbuf.h>
#include <glibmm/exception.h>

using namespace std;

class ImageManager
{

public:
    vector<Image> Images;
    future<void> get_all_images(const string &filterType);
    future<bool> add_image_async(const string &selectedFilePath);
    bool delete_image(const string &selectedFilePath);

protected:
    void load_image_file(const string &filterType);
    bool copy_image_file(const string &selectedFilePath);
    bool copy_image_linux(const string &selectedFilePath);
    bool copy_image_windows(const string &selectedFilePath);
    bool copy_image_macos(const string &selectedFilePath);
    bool delete_image_file(const string &selectedFilePath);
    bool delete_image_linux(const string &selectedFilePath);
    bool delete_image_windows(const string &selectedFilePath);
    bool delete_image_macos(const string &selectedFilePath);
    bool result_type(int res);

private:
    std::list<Magick::Image> frames;
};

#endif // IMAGE_MANAGER_H
