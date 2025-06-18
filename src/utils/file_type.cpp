#include "../../include/utils/file_type.h"

bool FileType::is_gif(const string &filePath)
{
    if (filePath.empty())
        return false;

    if (filePath.size() < 4)
    {
        if (filePath == "gif")
            return true;
    }

    return filePath.size() > 4 && filePath.substr(filePath.size() - 4) == ".gif";
}