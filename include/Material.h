#ifndef DW_RAYTRACER_MATERIAL_H
#define DW_RAYTRACER_MATERIAL_H

#include "Colour.h"
#include "Texture.h"

/* Material used for local illumination Phong lighting. */
class Material
{

public:
    Material(const Colour& ambient, const Colour& diffuse,
        const Colour& specular, Texture* texture = NULL);

    const Colour& getAmbient() const;
    const Colour& getDiffuse() const;
    const Colour& getSpecular() const;
    Texture* getTexture() const;
    void setAmbient(const Colour& newAmbient);
    void setDiffuse(const Colour& newDiffuse);
    void setSpecular(const Colour& newSpecular);
    void setTexture(Texture* newTexture);

private:
    Colour ambient;
    Colour diffuse;
    Colour specular;
    Texture* texture;

};

#endif
