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

	/* Add a shape as a child of the bounding shape. */
	void addShape(Shape* newShape);
	/* Remove shape which is currently a child of the bounding shape.
	 * If given shape is not a child, then it is silently ignored. */
	bool removeShape(Shape* shapeToRemove);

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
