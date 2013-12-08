#include "AABB.h"

AABB::AABB()
{
}

AABB::AABB(const Vector3& boxMin, const Vector3& boxMax)
{
    bounds[0] = boxMin;
    bounds[1] = boxMax;
}

bool AABB::intersects(const Ray& ray, float tMin, float tMax) const
{
    int posNeg = ray.directionSigns[0];
    float t0 = (bounds[posNeg].x - ray.origin().x) * ray.inverseDirection().x;
}
