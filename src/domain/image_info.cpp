#include "../../include/domain/image_info.h"

Image::Image(const string &imgName, const string &path, int width, int height)
    : imageName(imgName), path(path), width(width), height(height)
{

    set_type(imgName);
}

string Image::get_image_name() const { return imageName; }
string Image::get_path() const { return path; }
string Image::get_type() const { return type; }
int Image::get_width() const { return width; }
int Image::get_height() const { return height; }

void Image::set_image_name(const string &name) { imageName = name; }
void Image::set_path(const string &path) { this->path = path; }
void Image::set_type(const string &filename)
{
    size_t pos = filename.find_last_of('.');
    if (pos != string::npos)
    {
        type = filename.substr(pos + 1);
    }
    else
    {
        type = "unknown";
    }
}
void Image::set_width(int width) { this->width = width; }
void Image::set_height(int height) { this->height = height; }
