#include "Light.h"

PointLight::PointLight(const Vector3& position, const Colour& ambient,
    const Colour& diffuse, const Colour& specular) :
    position(position), ambient(ambient), diffuse(diffuse),
    specular(specular)
{
}

bool PointLight::hit(const Ray& ray, float& intensity) const
{
    // TODO
}

const Colour& PointLight::getAmbient() const
{
    return ambient;
}

const Colour& PointLight::getDiffuse() const
{
    return diffuse;
}

const Colour& PointLight::getSpecular() const
{
    return specular;
}

void PointLight::setAmbient(const Colour& newAmbient)
{
    ambient = newAmbient;
}

void PointLight::setDiffuse(const Colour& newDiffuse)
{
    diffuse = newDiffuse;
}

void PointLight::setSpecular(const Colour& newSpecular)
{
    specular = newSpecular;
}
