#include "BoundingShape.h"

using namespace raytracer;

BoundingShape::BoundingShape(const ShapeList& children, AABB& boundingBox)
    : children(children), boundingBox(boundingBox)
{
    centrePoint = boundingBox.bounds[0] + ((boundingBox.bounds[1] - boundingBox.bounds[0]) / 2);
}

BoundingShape::~BoundingShape()
{
    for (int i = 0; (i < children.size()); i++)
    {
        delete children[i];
    }
}

const Vector3& BoundingShape::getCentre() const
{
    return centrePoint;
}

void BoundingShape::addShape(Shape* newShape)
{
	children.push_back(newShape);
}

bool BoundingShape::removeShape(Shape* shapeToRemove)
{
	children.erase( std::remove(children.begin(), children.end(), shapeToRemove), children.end() );
}

bool BoundingShape::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    // First check if ray intersects with the bounding box
    if (!boundingBox.intersects(ray, tMin, tMax))
        return false;
        
    bool isAHit = false;
    for (int i = 0; (i < children.size()); i++)
    {
        // Keeping tMax up-to-date ensures that that only the colour
        // of the CLOSEST point is considered at the end of the loop
        if (children[i]->hit(ray, tMin, tMax, 0.0f, record))
        {
            // New maximum allowed distance becomes distance of this shape
            tMax = record.t;
            isAHit = true;
        }
    }
    return isAHit;
}

bool BoundingShape::shadowHit(const Ray& ray, float tMin, float tMax,
    float time, const Shape*& occludingShape) const
{
    // First check if ray intersects with the bounding box
    if (!boundingBox.intersects(ray, tMin, tMax))
        return false;

    for (int i = 0; (i < children.size()); i++)
        // NOTE: We only care if ANY shape is hit by the ray
        if (children[i]->shadowHit(ray, tMin, tMax, 0.0f, occludingShape))
            return true;
    return false;
}
