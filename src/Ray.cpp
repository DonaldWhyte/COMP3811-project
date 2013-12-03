#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3& origin, const Vector3& direction)
    : origin(origin), direction(direction)
{
}

Vector3 Ray::pointAtParameter(float t) const
{
    return origin + (t * direction);
}
