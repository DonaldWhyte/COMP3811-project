#ifndef DW_RAYTRACER_COMMON_H
#define DW_RAYTRACER_COMMON_H

#include "Vector3.h"
#include "Ray.h"
#include "Shape.h"
#include "Mesh.h"

namespace common
{

    /* Triangle intersection test with raytrace hit record updating. Separated
     * here so it can be used in multiple shape classes. */
    bool triangleHit(const Vector3& p1, const Vector3& p2, const Vector3& p3,
        const Ray& ray, float tMin, float tMax, float time, HitRecord& record);
    bool triangleShadowHit(const Vector3& p1, const Vector3& p2,
        const Vector3& p3, const Ray& ray, float tMin, float tMax, float time);
    /* A spearate triangle intersection test is used for generic vertices, which
     * contain vertex attributes other than position, so those attributes
     * be interpolated correctly (e.g. texture coordinate). */
    bool triangleHit(const Vertex& v1, const Vertex& v2, const Vertex& v3,
        const Ray& ray, float tMin, float tMax, float time, HitRecord& record);

}

#endif
