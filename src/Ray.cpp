#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3& rOrigin, const Vector3& rDirection)
{
    setOrigin(rOrigin);
    setDirection(rDirection);
}

Vector3 Ray::pointAtParameter(float t) const
{
    return rOrigin + (t * rDirection);
}

const Vector3& Ray::origin() const
{
    return rOrigin;
}

const Vector3& Ray::direction() const
{
    return rDirection;
}

const Vector3& Ray::inverseDirection() const
{
    return rInverseDirection;
}

void Ray::setOrigin(const Vector3& newOrigin)
{
    rOrigin = newOrigin;
}

void Ray::setDirection(const Vector3& newDirection)
{
    rDirection = newDirection;
    // Compute inverse of ray direction
    rInverseDirection = Vector3(1.0f / rDirection.x, 1.0f / rDirection.y, 1.0f / rDirection.z);
    // Update ray direction sings
    directionSigns[0] = (rDirection.x > 0 ? 0 : 1);
    directionSigns[1] = (rDirection.y > 0 ? 0 : 1);
    directionSigns[2] = (rDirection.z > 0 ? 0 : 1);
}
