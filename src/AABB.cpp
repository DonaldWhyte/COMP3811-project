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
    float intervalMin = tMin;
    float intervalMax = tMax;
    const Vector3& origin = ray.origin();
    const Vector3& inverseDirection = ray.inverseDirection();

    int posNeg = ray.directionSigns[0];
    float t0 = (bounds[posNeg].x - origin.x) * inverseDirection.x;
    float t1 = (bounds[1 - posNeg].x - origin.x) * inverseDirection.x;
    if (t0 > intervalMin) intervalMin = t0;
    if (t1 < intervalMax) intervalMax = t1;
    if (intervalMin > intervalMax) return false;

    posNeg = ray.directionSigns[1];
    t0 = (bounds[posNeg].y - origin.y) * inverseDirection.y;
    t1 = (bounds[1 - posNeg].y - origin.y) * inverseDirection.y;
    if (t0 > intervalMin) intervalMin = t0;
    if (t1 < intervalMax) intervalMax = t1;
    if (intervalMin > intervalMax) return false;

    posNeg = ray.directionSigns[2];
    t0 = (bounds[posNeg].z - origin.z) * inverseDirection.z;
    t1 = (bounds[1 - posNeg].z - origin.z) * inverseDirection.z;
    if (t0 > intervalMin) intervalMin = t0;
    if (t1 < intervalMax) intervalMax = t1;
    return (intervalMin <= intervalMax);
}
