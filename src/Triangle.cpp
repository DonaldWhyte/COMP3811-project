#include "Triangle.h"
#include "Common.h"

Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material) :
    p1(p1), p2(p2), p3(p3)
{
    this->material = material;
}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    bool isHit = common::triangleHit(p1, p2, p3, ray, tMin, tMax, time, record);
    // Augment hit record with the material of the hit shape
    if (isHit)
        record.material = material;
    return isHit;
}

bool Triangle::shadowHit(const Ray& ray, float tMin, float tMax, float time) const
{
    return common::triangleShadowHit(p1, p2, p3, ray, tMin, tMax, time);
}
