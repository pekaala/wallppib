#ifndef IMAGE_H
#define IMAGE_H

#include <string>

using namespace std;

class Image
{
public:
    Image(const string &imgName, const string &path, int width = 0, int height = 0)
        : imageName(imgName), path(path), width(width), height(height)
    {
        setType(imgName);
    }

    // Getters
    string getImageName() const { return imageName; }
    string getPath() const { return path; }
    string getType() const { return type; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Setters
    void setImageName(const string &name) { imageName = name; }
    void setPath(const string &path) { this->path = path; }
    void setType(const string &name)
    {
        size_t pos = name.find_last_of('.');
        if (pos != string::npos)
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
    string imageName;
    string path;
    string type;
    int width;
    int height;
};

#endif // IMAGE_H
