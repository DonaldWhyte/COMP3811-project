#include "Image.h"
#include <cmath>
#include <fstream>

struct TGAColour
{
    unsigned char b, g, r;
};

TGAColour toTGAColour(const Colour& colour)
{
    TGAColour tgaColour = {
        static_cast<unsigned char>(colour.b * 255),
        static_cast<unsigned char>(colour.g * 255),
        static_cast<unsigned char>(colour.r * 255)
    };
    return tgaColour;
}

Image::Image(int width, int height, const Colour& background)
    : width(width), height(height)
{
    // Allocate fixed-size array to store pixels
    pixels.resize(width); // number of rows
    for (unsigned int i = 0; (i < pixels.size()); i++)
    {
        pixels[i].resize(height); // number of columns
    }
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

/* TGA writing code written with the help of:
 * http://www.paulbourke.net/dataformats/tga/
*/
void Image::toFile(const std::string& filename)
{
    std::ofstream imageFile(filename.c_str(), std::ios::out | std::ios::binary);
    // Write TGA header and write it
 	imageFile.put(0);
   	imageFile.put(0);
   	// Data format (uncompressed RGB)
   	imageFile.put(2);
   	imageFile.put(0); imageFile.put(0);
   	imageFile.put(0); imageFile.put(0);
   	imageFile.put(0);
   	// X and Y origin
   	imageFile.put(0); 	imageFile.put(0);
   	imageFile.put(0); 	imageFile.put(0);
   	// Image dimensions
   	imageFile.put((width & 0x00FF));
   	imageFile.put((width & 0xFF00) / 256);
   	imageFile.put((height & 0x00FF));
   	imageFile.put((height & 0xFF00) / 256);
   	// 24 bits per pixel
   	imageFile.put(24);
   	imageFile.put(0);

    // Write uncompressed RGB pixel data
    TGAColour temp;
    for (int y = (height - 1); (y >= 0); y--) // going from bottom-to-top
    {
        for (int x = 0; (x < width); x++)
        {
            temp = toTGAColour(pixels[x][y]);
            imageFile.write(reinterpret_cast<char*>(&temp), sizeof(TGAColour));
        }
    }
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}
