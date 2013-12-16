#ifndef DW_RAYTRACER_MATERIAL_H
#define DW_RAYTRACER_MATERIAL_H

#include "Colour.h"
#include "Texture.h"

namespace raytracer {

/* Material used for local illumination Phong lighting. */
class Material
{

public:
    /* Useful constants for setting material parameters. */
    static const float NO_REFLECTION = -1.0f; // value to indicate to raytracer NOT TO reflect
    static const float NO_REFRACTION = -1.0f; // value to indicate to raytracer NOT TO refract
    // Refractive index of air.
    // (source: http://en.wikipedia.org/wiki/Refractive_index)
    static const float AIR_REFRACTIVE_INDEX = 1.000293;

    Material();
    Material(float ka, float kd, float ks, float n, float reflectionFactor,
        float refractiveIndex, const Colour& colour, Texture* texture = NULL);

    float ambientIntensity() const;
    float diffuseIntensity() const;
    float specularIntensity() const;
    float specularExponent() const;
    float reflectivity() const;
    float refractiveIndex() const;
    const Colour& getColour() const;
    Texture* getTexture() const;
    void setAmbientIntensity(float newKA);
    void setDiffuseIntensity(float newKD);
    void setSpecularIntensity(float newKS);
    void setSpecularExponent(float n);
    void setReflectivity(float newReflectionFactor);
    void setRefractiveIndex(float newRefractiveIndex);
    void setColour(const Colour& newAmbient);
    void setTexture(Texture* newTexture);

private:
    // Intensities for different lighting phenomena
    float ka; // ambient
    float kd; // diffuse
    float ks; // specular
    float n; // specular exponent
    // How much light this object reflects (0 = no reflection, 1 = mirror)
    float reflectionFactor;
    // This determines how the surface refracts light
    float surfaceRefractiveIndex;

    Colour colour;
    Texture* texture;

};

}

#endif
