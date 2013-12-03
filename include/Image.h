#ifndef DW_RAYTRACER_IMAGE_H
#define DW_RAYTRACER_IMAGE_H

#include <vector>
#include <string>
#include "Colour.h"

class Image
{

public:
    Image(int width, int height, const Colour& background = Colour(0.0f, 0.0f, 0.0f));

    bool set(int x, int y, const Colour& colour);
    void gammaCorrect(float gamma);

    void toFile(const std::string& filename);
    void fromFile(const std::string& filename);

private:
    std::vector<std::vector<Colour> > pixels; // accessed pixels[row][column]
    int width;
    int height;

};

#endif
