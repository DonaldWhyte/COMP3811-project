#include "Triangle.h"

Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Colour& colour) :
    p1(p1), p2(p2), p3(p3), colour(colour)
{
    //
}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    // Using Cramer's rule to efficiently find beta, gamma and t
    // For this, the 3 x 3 matrix below is constructed;
    float a = p1.x - p2.x; // column 1
    float b = p1.y - p2.y;
    float c = p1.z - p2.z;
    float d = p1.x - p3.x; // column 2
    float e = p1.y - p3.y;
    float f = p1.y - p3.z;
    float g = ray.direction.x; // column 3
    float h = ray.direction.y;
    float i = ray.direction.z;
    float j = p1.x - ray.origin.x; // RHS of system of equations to solve
    float k = p1.y - ray.origin.y;
    float l = p1.z - ray.origin.z;

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
    if (beta <= 0.0f || beta >= 1.0f)
        return false;
    // Compute gamma Barycentric coordinates
    float akjb = (a * k) - (j * b);
    float jcal = (j * c) - (a * l);
    float blkc = (b * l) - (k * c);
    float gamma = (i * akjb + h * jcal + g * blkc) / denominator;
    if (gamma <= 0.0f || (beta + gamma) >= 1.0f)
        return false;
    // Compute 'time' that ray hits the triangle
    float tVal = -(f * akjb + e * jcal + d*blkc) / denominator;

    // If ray hits triangle too late (i.e. it's too far!) then we
    // consider it as no intersection
    if (tVal >= tMin && tVal <= tMax)
    {
        record.t = tVal;
        record.normal = (p2 - p1).cross(p3 - p1).normalise();
        record.colour = colour;
        return true;
    }

    return false;
}

bool Triangle::shadowHit(const Ray& ray, float tMin, float tMax, float time) const
{

}
