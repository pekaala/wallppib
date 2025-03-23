#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <string>

class ImageInfo
{
public:
    ImageInfo(const std::string &imgName, const std::string &path, int width = 0, int height = 0)
        : imageName(imgName), path(path), width(width), height(height)
    {
        setType(imgName);
    }

    // Getters
    std::string getImageName() const { return imageName; }
    std::string getPath() const { return path; }
    std::string getType() const { return type; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Setters
    void setImageName(const std::string &name) { imageName = name; }
    void setPath(const std::string &path) { this->path = path; }
    void setType(const std::string &name)
    {
        size_t pos = name.find_last_of('.');
        if (pos != std::string::npos)
        {
            type = name.substr(pos + 1);
        }
        else
        {
            type = "";
        }
    }
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }

private:
    std::string imageName;
    std::string path;
    std::string type;
    int width;
    int height;
};

#endif // IMAGEINFO_H
