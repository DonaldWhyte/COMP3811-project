#ifndef DW_RAYTRACER_LIGHT_H
#define DW_RAYTRACER_LIGHT_H

#include <vector>
#include "Colour.h"
#include "Vector3.h"
#include "Ray.h"

class Light
{

public:
    virtual bool hit(const Ray& ray, float& intensity) const = 0;

};

typedef std::vector<Light*> LightList;

class PointLight : public Light
{

public:
    PointLight(const Vector3& position, const Colour& ambient,
        const Colour& diffuse, const Colour& specular);

    bool hit(const Ray& ray, float& intensity) const;

    const Colour& getAmbient() const;
    const Colour& getDiffuse() const;
    const Colour& getSpecular() const;
    void setAmbient(const Colour& newAmbient);
    void setDiffuse(const Colour& newDiffuse);
    void setSpecular(const Colour& newSpecular);

private:
    Vector3 position;
    // TODO: attenuation

    Colour ambient;
    Colour diffuse;
    Colour specular;

};

#endif
