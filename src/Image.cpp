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

void Image::setWidth(int newWidth)
{
	if (newWidth <= 0)
		return;
	pixels.resize(newWidth);
	width = newWidth;
}

void Image::setHeight(int newHeight)
{
	if (newHeight <= 0)
		return;
	// For every column, change height
    for (unsigned int i = 0; (i < pixels.size()); i++)
        pixels[i].resize(newHeight);
    height = newHeight;
}

void Image::resize(int newWidth, int newHeight)
{
	setWidth(newWidth);
	setHeight(newHeight);
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
