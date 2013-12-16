#ifndef DW_RAYTRACER_IMAGE_H
#define DW_RAYTRACER_IMAGE_H

#include <vector>
#include <string>
#include "Colour.h"

namespace raytracer {

class Image
{

public:
    Image(int width, int height, const Colour& background = Colour(0.0f, 0.0f, 0.0f));
    static Image fromFile(const std::string& filename);

    void clear(const Colour& colour);
    bool set(int x, int y, const Colour& colour);
    void gammaCorrect(float gamma);

    const Colour& get(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

private:
    std::vector<std::vector<Colour> > pixels; // accessed pixels[row][column]
    int width;
    int height;

};

}

#endif
