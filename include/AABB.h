#ifndef DW_RAYTRACER_AABB_H
#define DW_RAYTRACER_AABB_H

class AABB
{

public:
    AABB();
    AABB(const Vector3& boxMin, const Vector3& boxMax);

    bool intersects(const Ray& ray, float tMin, float tMax) const;

    Vector3 boxMin, boxMax;

};

#endif
