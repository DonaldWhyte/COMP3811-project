#ifndef DW_RAYTRACER_MATERIAL_H
#define DW_RAYTRACER_MATERIAL_H

#include "Colour.h"
#include "Texture.h"

/* Material used for local illumination Phong lighting. */
class Material
{

public:
    Material(float ka, float kd, float ks, float n,
        const Colour& colour, Texture* texture = NULL);

    float ambientIntensity() const;
    float diffuseIntensity() const;
    float specularIntensity() const;
    float specularExponent() const;
    const Colour& getColour() const;
    Texture* getTexture() const;
    void setAmbientIntensity(float newKA);
    void setDiffuseIntensity(float newKD);
    void setSpecularIntensity(float newKS);
    void setSpecularExponent(float n);
    void setColour(const Colour& newAmbient);
    void setTexture(Texture* newTexture);

private:
    // Intensities for different lighting phenomena
    float ka; // ambient
    float kd; // diffuse
    float ks; // specular
    float n; // specular exponent
    Colour colour;
    Texture* texture;

};

#endif
