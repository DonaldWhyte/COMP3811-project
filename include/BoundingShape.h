#ifndef DW_RAYTRACER_BOUNDINGSHAPE_H
#define DW_RAYTRACER_BOUNDINGSHAPE_H

#include "Shape.h"
#include "AABB.h"

namespace raytracer {

class BoundingShape : public Shape
{

public:
    BoundingShape(const ShapeList& shapes, AABB& boundingBox);
    virtual ~BoundingShape();

    virtual const Vector3& getCentre() const;
    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const;
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax, float time, const Shape*& occludingShape) const;

private:
    ShapeList children;
    Vector3 centrePoint;
    AABB boundingBox;

};

}

#endif
