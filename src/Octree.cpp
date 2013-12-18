#include "Octree.h"

using namespace raytracer;

Octree::Octree(const AABB& boundary) : boundary(boundary), numShapes(0), numChildren(0)
{
    // Initialise all shapes and child nodes to NULL
    clearShapes();
    for (int i = 0; (i < MAX_CHILDREN); i++)
        children[i] = NULL;
    // Compute centre point of bounding box
    centre = boundary.bounds[0] + ((boundary.bounds[1] - boundary.bounds[0]) / 2);
}

Octree::~Octree()
{
    clearChildren();
}

void Octree::clearShapes()
{
    for (int i = 0; (i < OCTREE_NODE_CAPACITY); i++)
        shapes[i] = NULL;
    numShapes = 0;
}

void Octree::clearChildren()
{
    for (int i = 0; (i < MAX_CHILDREN); i++)
        delete children[i];
    for (int i = 0; (i < MAX_CHILDREN); i++)
        children[i] = NULL;
}

bool Octree::insert(Shape* shape)
{
    // If shape is not contained within this bounding box, doesn't add it
    if (!boundary.contains(shape->getCentre()))
    {
        return false;
    }
    // If there is more space to put shapes in this node, add the shape
    else if (numShapes < OCTREE_NODE_CAPACITY)
    {
        shapes[numShapes] = shape;
        numShapes++;
        return true;
    }
    // Otherwise, divide node into 8 sub-nodes and add the shape to the correct child
    else
    {
        if (numChildren == 0)
            subdivide();
        for (unsigned int i = 0; (i < numChildren); i++)
            if (children[i]->insert(shape))
                return true;
        // If this is reached, point could not be inserted.
        // NOTE: THIS CODE SHOULD NEVER BE REACHED!
        // It's here to make the compiler happy.
        return false;
    }
}

void Octree::subdivide()
{
    // Construct AABBs for each of the children (all same size)
    const Vector3& min = boundary.bounds[0];
    const Vector3& max = boundary.bounds[1];
    AABB childrenBoundaries[MAX_CHILDREN] = {
        AABB( Vector3(min.x, min.y, min.z), Vector3(centre.x, centre.y, centre.z) ),
        AABB( Vector3(centre.x, min.y, min.z), Vector3(max.x, centre.y, centre.z) ),
        AABB( Vector3(min.x, centre.y, min.z), Vector3(centre.x, max.y, centre.z) ),
        AABB( Vector3(min.x, min.y, centre.z), Vector3(centre.x, centre.y, max.z) ),
        AABB( Vector3(centre.x, centre.y, min.z), Vector3(max.x, max.y, centre.z) ),
        AABB( Vector3(min.x, centre.y, centre.z), Vector3(centre.x, max.y, max.z) ),
        AABB( Vector3(centre.x, min.y, centre.z), Vector3(max.x, centre.y, max.z) ),
        AABB( Vector3(centre.x, centre.y, centre.z), Vector3(max.x, max.y, max.z) )
    };
    // Create the eight children
    for (unsigned int i = 0; (i < MAX_CHILDREN); i++)
        children[i] = new Octree(childrenBoundaries[i]);
    numChildren = MAX_CHILDREN;
    // For all the shapes currently in this node, move them to one of the children
    unsigned int shapesMoved = 0;
    for (unsigned int shapeIndex = 0; (shapeIndex < numShapes); shapeIndex++)
    {
        for (unsigned int childIndex = 0; (childIndex < numChildren); childIndex++)
        {
            // If true is returned, then we've inserted the shape, so we
            // move onto the next by breaking out of child-loop
            Octree* child = children[childIndex];
            if (child->insert(shapes[shapeIndex]))
            {
            	shapesMoved++;
                break;
            }
        }
    }
    clearShapes(); // remove shapes from this node (no longer leaf with shapes)
}

LineList Octree::getBoundingLines()
{
    LineList lines = generateLinesFromBox(boundary);
    for (unsigned int i = 0; (i < numChildren); i++)
    {
        LineList childLines = children[i]->getBoundingLines();
        lines.reserve(lines.size() + childLines.size()); // pre-allocate required memoery for concatenation
        lines.insert(lines.end(), childLines.begin(), childLines.end());
    }
    return lines;
}

const Vector3& Octree::getCentre() const
{
    return centre;
}

bool Octree::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    // First check if ray intersects with the bounding box of this node
    if (!boundary.intersects(ray, tMin, tMax))
        return false;

    bool isAHit = false;
    for (int i = 0; (i < numChildren); i++)
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
    if (!isAHit)
    {
        for (int i = 0; (i < numShapes); i++)
        {
            if (shapes[i]->hit(ray, tMin, tMax, 0.0f, record))
            {
                tMax = record.t;
                isAHit = true;
            }
        }
    }
    return isAHit;
}

bool Octree::shadowHit(const Ray& ray, float tMin, float tMax, float time, const Shape*& occludingShape) const
{
    if (!boundary.intersects(ray, tMin, tMax))
        return false;
    for (int i = 0; (i < numChildren); i++)
        if (children[i]->shadowHit(ray, tMin, tMax, 0.0f, occludingShape))
            return true;
    return false;
}


/* Define mock shape simply for testing purposes. */
class MockShape : public Shape
{
public:
    MockShape(const Vector3& centrePoint) : centrePoint(centrePoint) { }

    virtual const Vector3& getCentre() const
    { return centrePoint; }
    virtual bool hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
    { return false; }
    virtual bool shadowHit(const Ray& ray, float tMin, float tMax, float time, const Shape*& occludingShape) const
    { return false; }

    Vector3 centrePoint;
};

void tests::testOctree()
{
    // Create mock shapes for tests
    MockShape shapes[] = {
        MockShape(Vector3(0, 0, 0)), // should be added
        MockShape(Vector3(-1, -3, 4)), // should be added
        MockShape(Vector3(0, 3, 1)), // should be added
        MockShape(Vector3(2, -3, 4)), // should be added
        MockShape(Vector3(6, 5, 5)), // should not be added
        MockShape(Vector3(-6, 5, 5)), // should not be added
    };
    // Create octree and add shapes to it to test how it behaves
    Octree octree(AABB( Vector3(-5, -5, -5), Vector3(5, 5, 5) ));

    // Test bounding box check
    if (!octree.insert(&shapes[0]))
        std::cout << "Shape at " << shapes[0].getCentre() << " was not added when it should have been!" << std::endl;
    if (octree.numShapes != 1)
        std::cout << "numShape not incremented when shape successfully added." << std::endl;
    if (octree.insert(&shapes[5]))
        std::cout << "Shape at " << shapes[5].getCentre() << "was added when it shouldn't have been!" << std::endl;

    // Test that node is sub-divided when node capacity is reached
    for (unsigned int i = 0; (i < Octree::OCTREE_NODE_CAPACITY - 1); i++)
        octree.insert(&shapes[2]);
    if (!octree.insert(&shapes[1]))
        std::cout << "Shape at " << shapes[1].getCentre() << " was not added when it should have been!" << std::endl;
    //Test correct number of children were created
    if (octree.numChildren != 8)
        std::cout << "Octree was not divided!" << std::endl;
    // Test shapes were all moved to other children
    if (octree.numShapes != 0)
        std::cout << "There should be no shapes in Octree root!" << std::endl;
    if (octree.children[5]->numShapes != (Octree::OCTREE_NODE_CAPACITY - 1))
        std::cout << "Shapes not correctly allocated to children (multiple shapes)." << std::endl;
    if (octree.children[3]->numShapes != 1 || octree.children[3]->shapes[0] != &shapes[1])
        std::cout << "Shapes not correctly allocated to children (single shape)." << std::endl;
}
