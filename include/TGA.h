#ifndef DW_RAYTRACER_TGAWRITER_H
#define DW_RAYTRACER_TGAWRITER_H

#include <string>
#include "Image.h"

namespace tga
{
    Image readTGAFile(const std::string& filename);
    void writeTGAFile(const std::string& filename, const Image& image);
}

#endif
