#ifndef DW_RAYTRACER_COMMON_H
#define DW_RAYTRACER_COMMON_H

#include "Vector3.h"
#include "Ray.h"
#include "Shape.h"
#include "Mesh.h"

namespace common
{

    /* A separate triangle intersection test is used for generic vertices, which
     * contain vertex attributes other than position, so those attributes
     * be interpolated correctly (e.g. texture coordinate).
     * NOTE: Credit to Shirley and Morley (Realistic Raytracing) for introducing
     * me to this type of method for triangle intersection. */
    inline bool triangleHit(const Vertex& v1, const Vertex& v2, const Vertex& v3,
        const Ray& ray, float tMin, float tMax, float time, HitRecord& record)
    {
        const Vector3& p1 = v1.position;
        const Vector3& p2 = v2.position;
        const Vector3& p3 = v3.position;

        Vector3 edge1 = p2 - p1;
        Vector3 edge2 = p3 - p1;

        Vector3 h = ray.direction().cross(edge2);
        float a = edge1.dot(h);

        if (a > -0.00001 && a < 0.00001)
            return false;

        // NOTE: Using division so multiplications can be used later (typically faster on CPU)
        float f = 1 / a;
        Vector3 s = ray.origin() - p1;

        float beta = f * s.dot(h);
        if (beta < 0.0 || beta > 1.0)
            return false;

        Vector3 q = s.cross(edge1);
        float gamma = f * ray.direction().dot(q);
        if (gamma < 0.0 || beta + gamma > 1.0)
            return false;

        float distance = f * edge2.dot(q);

        // Ray Intersection
        if (distance >= tMin && distance < tMax)
        {
            // Record distance ray intersected the triangle
            record.t = distance;
            // Compute third barycentric coordinate for texture coordinate and point of intersecion
            float alpha = 1.0f - beta - gamma;
            record.pointOfIntersection = p1 * alpha + p2 * beta + p3 * gamma;
            record.texCoord = v1.texCoord * alpha + v2.texCoord * beta + v3.texCoord * gamma;
            // Compute triangle normal
            record.normal = (p2 - p1).cross(p3 - p1).normalise();

            return true;
        }
        else
        {
            return false;
        }
    }

    /* Triangle intersection test with raytrace hit record updating. Separated
     * here so it can be used in multiple shape classes.
 */
    inline bool triangleHit(const Vector3& p1, const Vector3& p2, const Vector3& p3,
        const Ray& ray, float tMin, float tMax, float time, HitRecord& record)
    {
        Vertex v1, v2, v3;
        v1.position = p1;
        v2.position = p2;
        v3.position = p3;
        return triangleHit(v1, v2, v3, ray, tMin, tMax, time, record);
    }

    /* Triangle intersection test without hit surface information being recorded.
     * This information is not needed when computing shadows, so having a separate
     * test for shadow rays that DOES NOT compute unnecessary information reduces
     * computation required (and hopefully increases performance). */
    inline bool triangleShadowHit(const Vector3& p1, const Vector3& p2,
        const Vector3& p3, const Ray& ray, float tMin, float tMax, float time)
    {
        Vector3 edge1 = p2 - p1;
        Vector3 edge2 = p3 - p1;

        Vector3 h = ray.direction().cross(edge2);
        float a = edge1.dot(h);

        if (a > -0.00001 && a < 0.00001)
            return false;

        float f = 1 / a;
        Vector3 s = ray.origin() - p1;

        float beta = f * s.dot(h);
        if (beta < 0.0 || beta > 1.0)
            return false;

        Vector3 q = s.cross(edge1);
        float gamma = f * ray.direction().dot(q);
        if (gamma < 0.0 || beta + gamma > 1.0)
            return false;

        float distance = f * edge2.dot(q);

        // Ray Intersection
        if (distance >= tMin && distance < tMax)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Generate floating point number >= min && <= max. */
    float randomFloat(float min, float max);
    /* Generate random 3D vector inside a circle using Monte Carlo method. */
    Vector3 monteCarloDirection();

}

#endif
