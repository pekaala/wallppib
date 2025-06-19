#ifndef FILE_TYPE_H
#define FILE_TYPE_H

#include <string>

class FileType
{
public:
    static bool is_gif(const std::string &filePath);
};

#endif // FILE_TYPE_H
