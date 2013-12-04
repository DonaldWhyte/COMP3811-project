#include "TGA.h"
#include <fstream>
#include <stdio.h>
#include <string.h>

/* TGA reading code taken from an old project I made open-source
 * a few years ago. Source files can be found here:
 * https://code.google.com/p/parcel-game-engine/source/browse/src/TGAImage.h
 * https://code.google.com/p/parcel-game-engine/source/browse/src/TGAImage.cpp
*/

/* TGA writing code written with the help of:
 * http://www.paulbourke.net/dataformats/tga/
*/

/* Stores all the values contained in the header of the TGA image.
 * This is used to determine the algorithm to use when loading the
 * pixel data. */
struct TGAHeader
{
    /* How much memory the TGA identifier string takes. Identifier is not really useful,
     * so this is only used to know how many bytes we have to skip over before reading pixel data. */
    unsigned char idLength;
    // 0 for true colour images and 1 for colur mapped images.
    unsigned char colourMapType;
    // Determines which type of TGA image it is. Types are in the TGAType enumerator.
    unsigned char imageTypeCode;
    // Indexed/colour map specific data
    unsigned char colourMapSpec[5];

    unsigned short xOrigin; // Origin of the X value
    unsigned short yOrigin; // Origin of the Y value
    unsigned short width; // Width (in pixels) of image
    unsigned short height; // Height (in pixels) of image
    // The amount of bits each pixel uses. Holds values of 8, 16, 24, or 32.
    unsigned char bitsPerPixel;
    /* Holds many bit fields that need to be extracted. Such as pixel orientation,
     * do the pixels start at the top-left, bottom-right, etc. */
    unsigned char imageDescriptor;
};

/* Enum that contains the ID numbers for each type of TGA image.
 * Used to check which type is the loaded image is by checking a byte
 * inside the TGA Header. */
enum TGAType
{
    TGA_NoData = 0, // Blank targa file
    TGA_ColourMapped, // A colour-mapped/indexed colour file
    TGA_TrueColour, // True colour image
    TGA_Grayscale, // Blank and white image

    // Compressed versions of the above
    TGA_CompressedColourMapped = 9,
    TGA_CompressedTrueColour,
    TGA_CompressedGrayscale,
};

/* The data format of the image. */
enum TGAImageDataFormat
{
    TGAFORMAT_RGB = 0, // Has red, blue and green components
    TGAFORMAT_RGBA, // Has rgb valuesand an additonal alpha component
    TGAFORMAT_Luminance, // Is a grayscale image, singular values; 0 to 255
};

/* These constant variables are values of the flags that determine
 * where the image starts its pixel data. Used for checking a value
 * in the TGA Header to dtermien the origin of the data.*/
static const unsigned char bottomLeft = 0x00;
static const unsigned char bottomRight = 0x10;
static const unsigned char topLeft = 0x20;
static const unsigned char topRight = 0x30;

struct TGAColour
{
    unsigned char b, g, r;
};

struct TGAColourAlpha
{
    unsigned char b, g, r, a;
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

Colour toFloatColour(unsigned char r, unsigned char g, unsigned char b)
{
    return Colour(
        static_cast<float>(r) / 255.0f,
        static_cast<float>(g) / 255.0f,
        static_cast<float>(b) / 255.0f
    );
}

Image* tga::readTGAFile(const std::string& filename)
{
    std::ifstream imageFile(filename.c_str(), std::ios::binary);
    if (!imageFile.is_open()) return NULL;
    // Read the TGA header from the file and store in the header struct
    TGAHeader header;
    imageFile.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));

    /* Return NULL if the image is a colour map or has no data.
     * NOTE: This doesn't and probably never will support indexed colour maps. */
     if (header.imageTypeCode == TGA_NoData ||
        header.imageTypeCode == TGA_ColourMapped ||
        header.imageTypeCode == TGA_CompressedColourMapped)
        return NULL;

    // colourMode is 3 = BGR, colourMode is 4 = BGRA
    int colourMode = header.bitsPerPixel / 8;
    /* If colour mode is less than 3, then that means that there is less than
     * 24 bits per pixel, and this loader does not support that. */
    if (colourMode < 3)
        return NULL;

    // Calculate size of image in memory and allocate it
    int imageSize = (header.width * header.height * colourMode);
    std::vector<unsigned char> pixelData;
    pixelData.resize(imageSize);

    // Skip past the ID string if there is one
    if (header.idLength > 0)
        imageFile.seekg(header.idLength, std::ios::cur);

    // If file is NOT compressed, simply read the entire file and store it in the pixelData array
    if (header.imageTypeCode == TGA_TrueColour ||
        header.imageTypeCode == TGA_Grayscale)
    {
        imageFile.read(reinterpret_cast<char*>(&pixelData[0]), pixelData.size());
    }
    // If compressed
    else
    {
        unsigned int i = 0;
        unsigned char id, length;
        TGAColour colour;
        TGAColourAlpha alphaColour;

        while (i < imageSize)
        {
            id = imageFile.get();
            // Checks if this is run length data
            if (id >= 128)
            {
                // Find the run length
                length = (unsigned char)(id - 127);
                // Read next colour
                if (colourMode == 4) // if alpha data in image
                    imageFile.read(reinterpret_cast<char*>(&alphaColour), sizeof(TGAColourAlpha));
                else
                    imageFile.read(reinterpret_cast<char*>(&colour), sizeof(TGAColour));

                // Loop until the length is greater than 0
                while(length > 0)
                {
                    // Puts this pixel's colour in the pixel data array
                    if (colourMode == 4)
                    {
                        pixelData[i++] = alphaColour.b;
                        pixelData[i++] = alphaColour.g;
                        pixelData[i++] = alphaColour.r;
                        pixelData[i++] = alphaColour.a;
                    }
                    else
                    {
                        pixelData[i++] = colour.b;
                        pixelData[i++] = colour.g;
                        pixelData[i++] = colour.r;
                    }
                    --length;
                }
            }
            else
            {
                // The number of non RLE (compressed) pixels
                length = (unsigned char)(id + 1);

                while(length > 0)
                {
                    // Read next colour and store it in pixel data array
                    if (colourMode == 4) // if alpha data in image
                    {
                        imageFile.read(reinterpret_cast<char*>(&alphaColour), sizeof(TGAColourAlpha));
                        pixelData[i++] = alphaColour.b;
                        pixelData[i++] = alphaColour.g;
                        pixelData[i++] = alphaColour.r;
                        pixelData[i++] = alphaColour.a;
                    }
                    else
                    {
                        imageFile.read(reinterpret_cast<char*>(&colour), sizeof(TGAColour));
                        pixelData[i++] = colour.b;
                        pixelData[i++] = colour.g;
                        pixelData[i++] = colour.r;
                    }
                    --length;
                }
            }
        }
    }
    // We're done reading from the image, so we close the file
    imageFile.close();

    // Construct image object and set all pixels
    Image* image = new Image(header.width, header.height);
    int width = image->getWidth();
    int height = image->getHeight();
    for (int x = 0; (x < width); x++)
    {
        for (int y = 0; (y < height); y++)
        {
            unsigned char b = pixelData[ ((x + (width * y)) * colourMode) + 0 ];
            unsigned char g = pixelData[ ((x + (width * y)) * colourMode) + 1];
            unsigned char r = pixelData[ ((x + (width * y)) * colourMode) + 2];
            image->set(x, y, toFloatColour(r, g, b));
        }
    }
    return image;
}

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
   	imageFile.put(topLeft);

    // Write uncompressed RGB pixel data
    TGAColour temp;
    for (int y = 0; (y < height); y++)
    {
        for (int x = 0; (x < width); x++)
        {
            temp = toTGAColour(image.get(x, y));
            imageFile.write(reinterpret_cast<char*>(&temp), sizeof(TGAColour));
        }
    }
}
