#include "TGA.h"
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

/* TGA writing code written with the help of:
 * http://www.paulbourke.net/dataformats/tga/
*/
void tga::writeTGAFile(const std::string& filename, const Image& image)
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
   	int width = image.getWidth();
   	int height = image.getHeight();
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
            temp = toTGAColour(image.get(x, y));
            imageFile.write(reinterpret_cast<char*>(&temp), sizeof(TGAColour));
        }
    }
}
