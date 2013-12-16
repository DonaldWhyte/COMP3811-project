#include "Light.h"

using namespace raytracer;

PointLight::PointLight(const Vector3& position, const Colour& ambient,
    const Colour& diffuse, const Colour& specular) :
    position(position), ambient(ambient), diffuse(diffuse),
    specular(specular)
{
}

const Vector3& PointLight::getPosition() const
{
    return position;
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

void PointLight::setPosition(const Vector3& newPosition)
{
    position = newPosition;
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
