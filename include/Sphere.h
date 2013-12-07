#ifndef DW_RAYTRACER_SPHERE_H
#define DW_RAYTRACER_SPHERE_H

#include "Shape.h"
#include "Ray.h"
#include "Colour.h"

class Sphere : public Shape
{

public:
    Sphere(const Vector3& centre, float radius, const Colour& colour, Material* material = NULL);

    bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const;
    bool shadowHit(const Ray& ray, float tMin, float tMax, float time) const;

    Vector3 centre;
    float radius;

};


#endif
