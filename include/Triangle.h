#ifndef DW_RAYTRACER_TRIANGLE_H
#define DW_RAYTRACER_TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{

public:
    Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material = NULL);

    bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const;
    bool shadowHit(const Ray& ray, float tMin, float tMax, float time) const;

private:
    Vector3 p1, p2, p3;

};

#endif
