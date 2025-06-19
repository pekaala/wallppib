#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image
{
public:
    Image(const std::string &imgName, const std::string &path, int width = 0, int height = 0);

    // Getters
    std::string get_image_name() const;
    std::string get_path() const;
    std::string get_type() const;
    int get_width() const;
    int get_height() const;

    // Setters
    void set_image_name(const std::string &name);
    void set_path(const std::string &path);
    void set_type(const std::string &filename);
    void set_width(int width);
    void set_height(int height);

private:
    std::string imageName;
    std::string path;
    std::string type;
    int width;
    int height;
};

#endif // IMAGE_H