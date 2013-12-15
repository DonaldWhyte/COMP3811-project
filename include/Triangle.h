#ifndef DW_RAYTRACER_TRIANGLE_H
#define DW_RAYTRACER_TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{

public:
    Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material = NULL);

    const Vector3& getCentre() const;
    bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const;
    bool shadowHit(const Ray& ray, float tMin, float tMax,
        float time, const Shape*& occludingShape) const;

private:
    Vector3 p1, p2, p3;
    Vector3 centrePoint;

};

#endif
