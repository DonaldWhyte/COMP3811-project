#ifndef DW_RAYTRACER_MATERIAL_H
#define DW_RAYTRACER_MATERIAL_H

#include "Colour.h"
#include "Texture.h"

/* Material used for local illumination Phong lighting. */
class Material
{

public:
    Material();
    Material(float ka, float kd, float ks, float n, float reflectionFactor,
        float transparencyFactor, const Colour& colour, Texture* texture = NULL);

    float ambientIntensity() const;
    float diffuseIntensity() const;
    float specularIntensity() const;
    float specularExponent() const;
    float reflectivity() const;
    float transparency() const;
    const Colour& getColour() const;
    Texture* getTexture() const;
    void setAmbientIntensity(float newKA);
    void setDiffuseIntensity(float newKD);
    void setSpecularIntensity(float newKS);
    void setSpecularExponent(float n);
    void setReflectivity(float newReflectionFactor);
    void setTransparency(float newTransparencyFactor);
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
    // How transparent the material is (0 = opaque, 1 = completely transparent)
    float transparencyFactor;

    Colour colour;
    Texture* texture;

};

#endif
