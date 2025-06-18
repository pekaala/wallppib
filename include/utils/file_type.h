#ifndef FILE_TYPE_H
#define FILE_TYPE_H

#include <string>

using std::string;

class FileType
{
public:
    static bool is_gif(const string &filePath);
};

#endif // FILE_TYPE_H
