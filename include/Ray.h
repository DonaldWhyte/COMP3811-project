#ifndef DW_RAYTRACER_RAY_H
#define DW_RAYTRACER_RAY_H

#include "Vector3.h"

class Ray
{

public:
    Ray();
    Ray(const Vector3& origin, const Vector3& direction);
    Vector3 pointAtParameter(float t) const;
    
    Vector3 origin, direction;

private:

};

#endif
