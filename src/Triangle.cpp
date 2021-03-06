#include "Triangle.h"
#include "Intersection.h"

using namespace raytracer;

Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material) :
    p1(p1), p2(p2), p3(p3)
{
    centrePoint = (p1 + p2 + p3) / 3;
    this->material = material;
}

const Vector3& Triangle::getCentre() const
{
    return centrePoint;
}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    bool isHit = intersection::triangleHit(p1, p2, p3, ray, tMin, tMax, time, record);
    // Augment hit record with the material of the hit shape
    if (isHit)
        record.hitShape = this;
    return isHit;
}

bool Triangle::shadowHit(const Ray& ray, float tMin, float tMax,
    float time, const Shape*& occludingShape) const
{
    bool isHit = intersection::triangleShadowHit(p1, p2, p3, ray, tMin, tMax, time);
    if (isHit)
        occludingShape = this;
    return isHit;
}
