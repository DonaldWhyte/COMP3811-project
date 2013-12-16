#include "Image.h"
#include <cmath>

using namespace raytracer;

Image::Image(int width, int height, const Colour& background)
    : width(width), height(height)
{
    // Allocate fixed-size array to store pixels
    pixels.resize(width); // number of rows
    for (unsigned int i = 0; (i < pixels.size()); i++)
    {
        pixels[i].resize(height); // number of columns
    }
    clear(background);
}

void Image::clear(const Colour& colour)
{
    for (int x = 0; (x < width); x++)
        for (int y = 0; (y < height); y++)
            pixels[x][y] = colour;
}

bool Image::set(int x, int y, const Colour& colour)
{
    if (x < 0 || x >= width) return false;
    if (y < 0 || y >= height) return false;
    pixels[x][y] = colour;
    return true;
}

void Image::gammaCorrect(float gamma)
{
    Colour temp;
    float power = 1.0 / gamma;
    for (int i = 0; (i < width); i++)
    {
        for (int j = 0; (j < height); j++)
        {
            temp = pixels[i][j];
            pixels[i][j] = Colour(
                pow(temp.r, power),
                pow(temp.g, power),
                pow(temp.b, power));
        }
    }
}

const Colour& Image::get(int x, int y) const
{
    return pixels[x][y];
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}
