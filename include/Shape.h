#ifndef DW_RAYTRACER_SHAPE
#define DW_RAYTRACER_SHAPE

#include <vector>
#include "Ray.h"
#include "Vector3.h"
#include "Colour.h"

struct HitRecord
{
    float t;
    Vector3 normal;
    Colour colour;
};

class Shape
{

public:
    virtual ~Shape() { } // here since it's an abstract type
    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const = 0;
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax, float time) const = 0;

};

typedef std::vector<Shape*> ShapeList;

#endif
