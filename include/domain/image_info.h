#ifndef IMAGE_H
#define IMAGE_H

#include <string>

using namespace std;

class Image
{
public:
    Image(const string &imgName, const string &path, int width = 0, int height = 0);

    // Getters
    string get_image_name() const;
    string get_path() const;
    string get_type() const;
    int get_width() const;
    int get_height() const;

    // Setters
    void set_image_name(const string &name);
    void set_path(const string &path);
    void set_type(const string &filename);
    void set_width(int width);
    void set_height(int height);

private:
    string imageName;
    string path;
    string type;
    int width;
    int height;
};

#endif // IMAGE_H