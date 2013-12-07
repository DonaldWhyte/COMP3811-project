#ifndef DW_RAYTRACER_SHAPE
#define DW_RAYTRACER_SHAPE

#include <vector>
#include "Ray.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"
#include "Colour.h"

struct HitRecord
{
    float t; // distance from original ray
    Vector3 pointOfIntersection;
    Vector3 normal; // surface normal of point of intersection
    Vector2 texCoord; // (U, V) coordinates of point of intersection
    Material* material;
    Colour colour;
};

class Shape
{

public:
    Shape() : material(NULL) { }
    // Virtual because it's an abstract type
    virtual ~Shape() { delete material; }
    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const = 0;
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax, float time) const = 0;

    Material* getMaterial() { return material; }
    void setMaterial(Material* newMaterial) { material = newMaterial; }

protected:
    Material* material;

};

typedef std::vector<Shape*> ShapeList;

#endif
