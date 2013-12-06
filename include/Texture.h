#ifndef DW_RAYTRACER_TEXTURE_H
#define DW_RAYTRACER_TEXTURE_H

#include "Image.h"

class Texture
{

public:
    Texture(Image* sourceImage);

    const Colour& getTexel(float u, float v) const;
    int getWidth() const;
    int getHeight() const;

private:
    Image* sourceImage;

};

#endif
