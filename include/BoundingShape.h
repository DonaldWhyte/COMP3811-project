#ifndef DW_RAYTRACER_BOUNDINGSHAPE_H
#define DW_RAYTRACER_BOUNDINGSHAPE_H

#include "Shape.h"
#include "AABB.h"

class BoundingShape
{

public:
    BoundingShape(const ShapeList& shapes, AABB& boundingBox);
    virtual ~BoundingShape();

    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const;
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax, float time) const;

private:
    ShapeList children;
    AABB boundingBox;

};

#endif
