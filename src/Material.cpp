#include "Material.h"

Material::Material() : texture(NULL)
{
}

Material::Material(const Colour& ambient, const Colour& diffuse,
    const Colour& specular, Texture* texture) :
    ambient(ambient), diffuse(diffuse), specular(specular), texture(texture)
{
}

const Colour& Material::getAmbient() const
{
    return ambient;
}

const Colour& Material::getDiffuse() const
{
    return diffuse;
}

const Colour& Material::getSpecular() const
{
    return specular;
}

Texture* Material::getTexture() const
{
    return texture;
}

void Material::setAmbient(const Colour& newAmbient)
{
    ambient = newAmbient;
}

void Material::setDiffuse(const Colour& newDiffuse)
{
    diffuse = newDiffuse;
}

void Material::setSpecular(const Colour& newSpecular)
{
    specular = newSpecular;
}

void Material::setTexture(Texture* newTexture)
{
    texture = newTexture;
}
