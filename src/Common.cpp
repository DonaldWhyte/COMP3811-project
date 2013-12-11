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
    float a = p1.x - p2.x;
    float b = p1.y - p2.y;
    float c = p1.z - p2.z;
    float d = p1.x - p3.x;
    float e = p1.y - p3.y;
    float f = p1.y - p3.z;
    float g = ray.direction().x;
    float h = ray.direction().y;
    float i = ray.direction().z;
    float j = p1.x - ray.origin().x;
    float k = p1.y - ray.origin().y;
    float l = p1.z - ray.origin().z;

    float eihf = (e * i) - (h * f);
    float gfdi = (g * f) - (d * i);
    float dheg = (d * h) - (e * g);

    float denominator = (a * eihf + b * gfdi + c * dheg);
    float beta = (j * eihf + k * gfdi + l * dheg) / denominator;
    if (beta <= 0.0f || beta >= 1.0f)
        return false;

    float akjb = (a * k) - (j * b);
    float jcal = (j * c) - (a * l);
    float blkc = (b * l) - (k * c);
    float gamma = (i * akjb + h * jcal + g * blkc) / denominator;
    if (gamma <= 0.0f || (beta + gamma) >= 1.0f)
        return false;

    float tVal = -(f * akjb + e * jcal + d*blkc) / denominator;
    if (tVal >= tMin && tVal <= tMax)
        return true;
    else
        return false;
}

bool common::triangleHit(const Vertex& v1, const Vertex& v2, const Vertex& v3,
    const Ray& ray, float tMin, float tMax, float time, HitRecord& record)
{
    const Vector3& p1 = v1.position;
    const Vector3& p2 = v2.position;
    const Vector3& p3 = v3.position;
    // Using Cramer's rule to efficiently find beta, gamma and t
    // For this, the 3 x 3 matrix below is constructed;
    float a = p1.x - p2.x; // column 1
    float b = p1.y - p2.y;
    float c = p1.z - p2.z;
    float d = p1.x - p3.x; // column 2
    float e = p1.y - p3.y;
    float f = p1.y - p3.z;
    float g = ray.direction().x; // column 3
    float h = ray.direction().y;
    float i = ray.direction().z;
    float j = p1.x - ray.origin().x; // RHS of system of equations to solve
    float k = p1.y - ray.origin().y;
    float l = p1.z - ray.origin().z;

    // These expressions are used multiple times, so store them
    // (so they only need to be computed once (plus it makes the code more readable)
    float eihf = (e * i) - (h * f);
    float gfdi = (g * f) - (d * i);
    float dheg = (d * h) - (e * g);

    // Compute 'beta' Barycentric coordinate
    float denominator = (a * eihf + b * gfdi + c * dheg);
    // TODO: check for 0 value for divide-by-zero here?
    //        costly operation, but necessary???
    float beta = (j * eihf + k * gfdi + l * dheg) / denominator;
    // If resultant coordinates is out of bounds, then ray does NOT hit the triangle!
    if (beta < 0.0f || beta > 1.0f)
        return false;
    // Compute gamma Barycentric coordinates
    float akjb = (a * k) - (j * b);
    float jcal = (j * c) - (a * l);
    float blkc = (b * l) - (k * c);
    float gamma = (i * akjb + h * jcal + g * blkc) / denominator;
    if (gamma < 0.0f || (beta + gamma) > 1.0f)
        return false;
    // Compute distance from origin of ray to triangle
    float tVal = -(f * akjb + e * jcal + d*blkc) / denominator;

    // If ray hits triangle too late (i.e. it's too far!) then we
    // consider it as no intersection
    if (tVal >= tMin && tVal <= tMax)
    {
        record.t = tVal;
        // Compute actual point of intersection given barycentric coordinayes
        float alpha = 1.0f - beta - gamma;
        record.pointOfIntersection = p1 * alpha + p2 * beta + p3 * gamma;
        record.normal = (p2 - p1).cross(p3 - p1).normalise();
        record.texCoord = v1.texCoord * alpha + v2.texCoord * beta + v3.texCoord * gamma;

        return true;
    }

    return false;
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
