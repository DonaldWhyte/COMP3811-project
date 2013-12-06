#include "Texture.h"

Texture::Texture(Image* sourceImage) : sourceImage(sourceImage)
{
}

const Colour& Texture::getTexel(float u, float v) const
{
    // Convert UV coordinates into pixel coordinates on source image
    int width = getWidth();
    int height = getHeight();
    int pixelX = static_cast<int>(u * width) % width;
    int pixelY = static_cast<int>(v * height) % height;
    // Return colour of pixel at resultant coordinates
    return sourceImage->get(pixelX, pixelY);
}

int Texture::getWidth() const
{
    return sourceImage->getWidth();
}

int Texture::getHeight() const
{
    return sourceImage->getHeight();
}
