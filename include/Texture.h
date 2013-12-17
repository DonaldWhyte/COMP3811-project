#ifndef DW_RAYTRACER_TEXTURE_H
#define DW_RAYTRACER_TEXTURE_H

#include "Image.h"

namespace raytracer {

/* Interface to a texture. */
class Texture
{

public:
	virtual ~Texture() { }

    virtual const Colour& getTexel(float u, float v) const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	
};

/* Implementation of Texture interface which uses a single image. */
class ImageTexture : public Texture
{

public:
    ImageTexture(Image* sourceImage);

    const Colour& getTexel(float u, float v) const;
    int getWidth() const;
    int getHeight() const;

private:
    Image* sourceImage;

};

}

#endif
