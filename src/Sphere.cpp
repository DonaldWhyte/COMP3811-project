#include "Sphere.h"
#include <cmath>

Sphere::Sphere(const Vector3& centre, float radius, Material* material) :
    centre(centre), radius(radius)
{
    this->material = material;
}

const Vector3& Sphere::getCentre() const
{
    return centre;
}

bool Sphere::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    Vector3 temp = ray.origin() - centre;

    // Solve quadratic equation to check for intersection
    double a = ray.direction().dot(ray.direction());
    double b = 2 * ray.direction().dot(temp);
    double c = temp.dot(temp) - (radius * radius);
    double discriminant = b * b - 4 * a * c;

    // Two stage check to see if ray intersects a sphere
    if (discriminant > 0) // stage one
    {
        discriminant = sqrt(discriminant);
        double t = (-b - discriminant) / (2 * a);
        if (t < tMin)
            t = (-b - discriminant) / (2 * a);
        // Check if ray is correct desired distance from sphere
        if (t < tMin || t > tMax)
        {
            return false;
        }
        else // ray hits the sphere!
        {
            // Update hit record here!
            record.t = t;
            record.pointOfIntersection = ray.origin() + (ray.direction() * t);
            record.normal = ray.origin() + (t * ray.direction()) - centre;
            record.normal = record.normal.normalise(); // normalise unit vector to get just direction
            // If refraction ray, reverse the normal
            if (ray.direction().dot(record.normal) > 0)
                record.normal = -record.normal;
            record.texCoord = computeTexCoord(record.pointOfIntersection);

            record.hitShape = reinterpret_cast<const Shape*>(this);
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Sphere::shadowHit(const Ray& ray, float tMin, float tMax,
    float time, const Shape*& occludingShape) const
{
    Vector3 temp = ray.origin() - centre;

    double a = ray.direction().dot(ray.direction());
    double b = 2 * ray.direction().dot(temp);
    double c = temp.dot(temp) - (radius * radius);

    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0)
    {
        discriminant = sqrt(discriminant);
        double t = (-b - discriminant) / (2 * a);
        if (t < tMin)
            t = (-b - discriminant) / (2 * a);
        if (t < tMin || t > tMax)
        {
            return false;
        }
        else
        {
            occludingShape = this;
            return true;
        }
    }
    else
    {
        return false;
    }
}

Vector2 Sphere::computeTexCoord(const Vector3& posOnSphere) const
{
    // Compute vector from position on sphere to sphere's origin
    Vector3 direction = (posOnSphere - centre).normalise();
    // Normalised as we only carew about direction
    float u = 0.5f + (atan2(direction.z, direction.x) / (2.0f * M_PI));
    float v = 0.5f - (asin(direction.y) / M_PI);
    return Vector2(u, v);
}
