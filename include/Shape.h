#ifndef DW_RAYTRACER_SHAPE
#define DW_RAYTRACER_SHAPE

#include <vector>
#include "Ray.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"
#include "Colour.h"

namespace raytracer {

class Shape; //forward declaration for 'hitShape' member
struct HitRecord
{
    float t; // distance from original ray
    Vector3 pointOfIntersection; // exact point in world coordinates that ray hit
    Vector3 normal; // surface normal of point of intersection
    Vector2 texCoord; // (U, V) coordinates of point of intersection

    const Shape* originShape; // shape which the ray bounced off
    const Shape* hitShape; // shape which the ray has hit

    Colour colour;

    /* Ensure numerical values and pointers are initialised 0
     * and NULL respectively. */
    HitRecord() : t (0), originShape(NULL), hitShape(NULL) { }

};

class Shape
{

public:
    Shape() : material(NULL) { }
    // Virtual because it's an abstract type
    virtual ~Shape() { delete material; }

    /* Retrieve centre point of the shape. */
    virtual const Vector3& getCentre() const = 0;
    /* Ray-shape intersection tests. */
    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const = 0;
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax,
        float time, const Shape*& occludingShape) const = 0;

    virtual const Material* getMaterial() const { return material; }
    virtual void setMaterial(Material* newMaterial) { material = newMaterial; }

protected:
    Material* material;

};

typedef std::vector<Shape*> ShapeList;

}

#endif
