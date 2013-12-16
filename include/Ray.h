#ifndef DW_RAYTRACER_RAY_H
#define DW_RAYTRACER_RAY_H

#include "Vector3.h"

namespace raytracer {

class Ray
{

public:
    inline Ray() { }

    inline Ray(const Vector3& rOrigin, const Vector3& rDirection)
    {
        setOrigin(rOrigin);
        setDirection(rDirection);
    }

    inline Vector3 pointAtParameter(float t) const
    {
        return rOrigin + (t * rDirection);
    }

    inline const Vector3& origin() const
    {
        return rOrigin;
    }

    inline const Vector3& direction() const
    {
        return rDirection;
    }

    inline const Vector3& inverseDirection() const
    {
        return rInverseDirection;
    }

    inline void setOrigin(const Vector3& newOrigin)
    {
        rOrigin = newOrigin;
    }

    inline void setDirection(const Vector3& newDirection)
    {
        rDirection = newDirection;
        // Compute inverse of ray direction
        rInverseDirection = Vector3(1.0f / rDirection.x, 1.0f / rDirection.y, 1.0f / rDirection.z);
        // Update ray direction sings
        directionSigns[0] = (rDirection.x > 0 ? 0 : 1);
        directionSigns[1] = (rDirection.y > 0 ? 0 : 1);
        directionSigns[2] = (rDirection.z > 0 ? 0 : 1);
    }

    // Sign of (X, Y, Z) components of directions.
    // These values are pre-computed so efficient bounding box
    // intersections can be performed. This strategy was taken
    // from the book Realistic Raytracing (Shirley, Morley)
    int directionSigns[3];

private:
    Vector3 rOrigin;
    Vector3 rDirection;
    // The ray's INVERSE direction is also pre-computed for efficiency
    Vector3 rInverseDirection;

};

}

#endif
