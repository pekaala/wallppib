
#ifndef IMAGES_H
#define IMAGES_H

#include <vector>
#include "ImageInfo.h"
#include "DirectoryPath.h"

DirectoryPath directoryPath;

class Images
{
    std::string dataStr = directoryPath.getDataContents();

public:
    const std::vector<ImageInfo> images{

        ImageInfo("wall_1.jpg", dataStr + "/wall_1.jpg", 1920, 1080),
        ImageInfo("wall_2.png", dataStr + "/wall_2.png", 1920, 1080),
        ImageInfo("wall_3.png", dataStr + "/wall_3.png", 1920, 1080),
        ImageInfo("wall_4.jpg", dataStr + "/wall_4.jpg", 1920, 1080),
        /* ImageInfo("wall_5.jpg", dataStr + "/wall_5.jpg", 1920, 1080),
        ImageInfo("wall_6.jpg", dataStr + "/wall_6.jpg", 1920, 1080),
        ImageInfo("test.gif", dataStr + "/test.gif", 1920, 1080), */
    };
};
#endif // IMAGES_H
