#ifndef DW_RAYTRACER_OCTREE_H
#define DW_RAYTRACER_OCTREE_H

#include "Shape.h"
#include "AABB.h"

/* Test octree code to ensure it's functioning correctly. */
namespace tests
{
    void testOctree();
}

class Octree : public Shape
{

    friend void tests::testOctree();

public:
    Octree(const AABB& boundingBox);
    virtual ~Octree();

    /* Return true if point is contained within this Octree node
     * and the point was added to the node or one of its children.
     * If false is returned, point was added to this node or any
     * of its children. */
    bool insert(Shape* shape);
    void subdivide();

    /* Remove all shapes from this node (recursively). */
    void clearShapes();
    /* Remove all children from this node. */
    void clearChildren();

    /* Implemented for Shape abstract class. */
    virtual const Vector3& getCentre() const;
    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const;
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax, float time, const Shape*& occludingShape) const;

private:
    static const unsigned int MAX_CHILDREN = 8;
    static const unsigned int OCTREE_NODE_CAPACITY = 16;

    AABB boundary; // region of space this octree node is for
    Vector3 centre; // centre point of region this node is for
    Shape* shapes[OCTREE_NODE_CAPACITY]; // shapes contained within this region
    Octree* children[8]; // all children of node

    unsigned int numShapes; // amount of shapes currently contained in this node
    unsigned int numChildren; // amount of children node currently has

};

#endif
