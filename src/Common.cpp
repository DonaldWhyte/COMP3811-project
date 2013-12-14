#include "Common.h"
#include <cstdlib>

bool common::triangleHit(const Vector3& p1, const Vector3& p2, const Vector3& p3,
    const Ray& ray, float tMin, float tMax, float time, HitRecord& record)
{
    Vertex v1, v2, v3;
    v1.position = p1;
    v2.position = p2;
    v3.position = p3;
    return triangleHit(v1, v2, v3, ray, tMin, tMax, time, record);
}

bool common::triangleShadowHit(const Vector3& p1, const Vector3& p2, const Vector3& p3,
    const Ray& ray, float tMin, float tMax, float time)
{
    Vector3 edge1 = p2 - p1;
    Vector3 edge2 = p3 - p1;

    Vector3 s1 = ray.direction().cross(edge2);
    float divisor = s1.dot(edge1);
    if (divisor == 0.0)
    {
           return false;
    }
    float invDivisor = 1/ divisor;

    Vector3 distance = ray.origin() - p1;
    float barycCoord_1 = distance.dot(s1) * invDivisor;
    if (barycCoord_1 < 0.0 || barycCoord_1 > 1.0)
        return false;
    Vector3 s2 = distance.cross(edge1);
    float barycCoord_2 = ray.direction().dot(s2) * invDivisor;
    if (barycCoord_2 < 0.0 || (barycCoord_1 + barycCoord_2) > 1.0)
        return false;

    return true;
}

bool common::triangleHit(const Vertex& v1, const Vertex& v2, const Vertex& v3,
    const Ray& ray, float tMin, float tMax, float time, HitRecord& record)
{
    const Vector3& p1 = v1.position;
    const Vector3& p2 = v2.position;
    const Vector3& p3 = v3.position;
    Vector3 edge1 = p2 - p1;
    Vector3 edge2 = p3 - p1;

    // Find the cross product of edge2 and the ray direction
    Vector3 s1 = ray.direction().cross(edge2);
    // Find the divisor, if its zero, return false as the triangle is
    // degenerated
    float divisor = s1.dot(edge1);
    if (divisor == 0.0)
    {
        return false;
    }
    // A inverted divisor, as multipling is faster then division
    float invDivisor = 1/ divisor;

    // Calculate the first barycentic coordinate. Barycentic coordinates
    // are between 0.0 and 1.0
    Vector3 distance = ray.origin() - p1;
    float barycCoord_1 = distance.dot(s1) * invDivisor;
    if (barycCoord_1 < 0.0 || barycCoord_1 > 1.0)
        return false;
    // Calculate the second barycentic coordinate
    Vector3 s2 = distance.cross(edge1);
    float barycCoord_2 = ray.direction().dot(s2) * invDivisor;
    if (barycCoord_2 < 0.0 || (barycCoord_1 + barycCoord_2) > 1.0)
        return false;

    // After doing the barycentic coordinate test we know if the ray hits or
    // not. If we got this far the ray hits.
    // Calculate the distance to the intersection point
    float intersectionDistance = edge2.dot(s2) * invDivisor;
    record.t = intersectionDistance;

    float alpha = 1.0f - barycCoord_1 - barycCoord_2;
    record.pointOfIntersection = p1 * alpha + p2 * barycCoord_1 + p3 * barycCoord_2;
    record.normal = (p2 - p1).cross(p3 - p1).normalise();
    record.texCoord = v1.texCoord * alpha + v2.texCoord * barycCoord_1 + v3.texCoord * barycCoord_2;

    return true;
}

float common::randomFloat(float min, float max)
{
    // Compute scalaing range
    float range = max - min;
    // Generate random number between [0..RAND_MAX]
    float randomNumber = static_cast<float>(rand());
    // Divide by maximum possible value to get in range [0..1]
    randomNumber /= static_cast<float>(RAND_MAX);
    // Scale by range and translate by minimum to get a random
    // float in the required range
    randomNumber *= range;
    randomNumber += min;
    return randomNumber;
}

Vector3 common::monteCarloDirection()
{
    while (true)
    {
        Vector3 vector(randomFloat(-1.0f, 1.0f),
            randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
        float length = vector.length();
        // If length of vector is bad, do another loop to generate another random vector
        if ((length > 1.0f) || (length < 0.1f))
            continue;
        // Normalise vector before returning so only direction is returned
        return vector.normalise();
    }
    return Vector3();
}
