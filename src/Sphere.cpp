#include "Sphere.h"
#include <cmath>

Sphere::Sphere(const Vector3& centre, float radius, const Colour& colour) :
    centre(centre), radius(radius), colour(colour)
{
}

bool Sphere::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    Vector3 temp = ray.origin - centre;
    
    double a = ray.direction.dot(ray.direction);
    double b = 2 * ray.direction.dot(temp);
    double c = temp.dot(temp) - (radius * radius);
    
    double discriminant = b * b - 4 * a * c; // quadratic formula discriminant
    
    // Two stage check to see if ray intersects a sphere
    if (discriminant > 0) // stage one
    {
        discriminant = sqrt(discriminant);
        double t = (-b - discriminant) / (2 * a);
        if (t < tMin)
            t = (-b - discriminant) / (2 * a);
        // Check if ray is within allowed proximity interval of sphee
        if (t < tMin || t > tMax)
        {
            return false;
        }
        else // ray hits the sphere!
        {
            // Update hit record here!
            record.t = t;
            record.normal = ray.origin + (t * ray.direction) - centre;
            record.normal = record.normal.normalise(); // normalise unit vector to get just direction
            record.colour = colour;
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Sphere::shadowHit(const Ray& ray, float tMin, float tMax, float time) const
{
    Vector3 temp = ray.origin - centre;
    
    double a = ray.direction.dot(ray.direction);
    double b = 2 * ray.direction.dot(temp);
    double c = temp.dot(temp) - (radius * radius);
    
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant > 0)
    {
        discriminant = sqrt(discriminant);
        double t = (-b - discriminant) / (2 * a);
        if (t < tMin)
            t = (-b - discriminant) / (2 * a);
        if (t < tMin || t > tMax)
            return false;
        else
            return true;
    }
    else
    {
        return false;
    }
}