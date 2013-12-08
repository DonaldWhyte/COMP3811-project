#ifndef DW_RAYTRACER_RAY_H
#define DW_RAYTRACER_RAY_H

#include "Vector3.h"

class Ray
{

public:
    Ray();
    Ray(const Vector3& rOrigin, const Vector3& rDirection);
    Vector3 pointAtParameter(float t) const;

    const Vector3& origin() const;
    const Vector3& direction() const;
    const Vector3& inverseDirection() const;
    void setOrigin(const Vector3& newOrigin);
    void setDirection(const Vector3& newDirection);

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

#endif
