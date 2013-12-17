#include "Texture.h"

using namespace raytracer;

ImageTexture::ImageTexture(Image* sourceImage) : sourceImage(sourceImage)
{
}

const Colour& ImageTexture::getTexel(float u, float v) const
{
    // Convert UV coordinates into pixel coordinates on source image
    int width = sourceImage->getWidth();
    int height = sourceImage->getHeight();
    int pixelX = static_cast<int>(u * width) % width;
    int pixelY = static_cast<int>(v * height) % height;
    // Return colour of pixel at resultant coordinates
    return sourceImage->get(pixelX, pixelY);
}
