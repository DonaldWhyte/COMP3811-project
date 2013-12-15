#ifndef DW_RAYTRACER_LIGHT_H
#define DW_RAYTRACER_LIGHT_H

#include <vector>
#include "Colour.h"
#include "Vector3.h"
#include "Ray.h"

class PointLight
{

public:
    PointLight(const Vector3& position, const Colour& ambient,
        const Colour& diffuse, const Colour& specular);

    const Vector3& getPosition() const;
    const Colour& getAmbient() const;
    const Colour& getDiffuse() const;
    const Colour& getSpecular() const;
    void setPosition(const Vector3& position);
    void setAmbient(const Colour& newAmbient);
    void setDiffuse(const Colour& newDiffuse);
    void setSpecular(const Colour& newSpecular);

private:
    Vector3 position;

    Colour ambient;
    Colour diffuse;
    Colour specular;

};

typedef std::vector<PointLight> LightList;

#endif
