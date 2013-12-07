#ifndef DW_RAYTRACER_COMMON_H
#define DW_RAYTRACER_COMMON_H

#include "Vector3.h"
#include "Ray.h"
#include "Shape.h"

namespace common
{

    bool triangleHit(const Vector3& p1, const Vector3& p2, const Vector3& p3,
        const Ray& ray, float tMin, float tMax, float time, HitRecord& record);
    bool triangleShadowHit(const Vector3& p1, const Vector3& p2,
        const Vector3& p3, const Ray& ray, float tMin, float tMax, float time);

}

#endif
