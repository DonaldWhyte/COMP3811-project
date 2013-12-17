#include "MeshTriangle.h"
#include "Intersection.h"

using namespace raytracer;

MeshTriangle::MeshTriangle(Mesh* mesh, int v1, int v2, int v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3)
{
    // Compute centre point of triangle
    const VertexList& vertices = mesh->getVertices();
    const Vector3& p1 = vertices[v1].position;
    const Vector3& p2 = vertices[v2].position;
    const Vector3& p3 = vertices[v3].position;
    centrePoint = (p1 + p2 + p3) / 3;
}

MeshTriangle::~MeshTriangle()
{
}

const Material* MeshTriangle::getMaterial() const
{
    return mesh->getMaterial();
}

void MeshTriangle::setMaterial(Material* newMaterial)
{
    // DO NOTHING
}

const Vector3& MeshTriangle::getCentre() const
{
    return centrePoint;
}

bool MeshTriangle::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    // Retrieve points from mesh
    const VertexList& vertices = mesh->getVertices();
    const Vertex& p1 = vertices[v1];
    const Vertex& p2 = vertices[v2];
    const Vertex& p3 = vertices[v3];
    // Perform ray-triangle intersection test
    bool isHit = intersection::triangleHit(p1, p2, p3, ray, tMin, tMax, time, record);
    if (isHit)
        record.hitShape = this;
    return isHit;
}

bool MeshTriangle::shadowHit(const Ray& ray, float tMin, float tMax, float time, const Shape*& occludingShape) const
{
    // Retrieve points from mesh
    const VertexList& vertices = mesh->getVertices();
    const Vertex& p1 = vertices[v1];
    const Vertex& p2 = vertices[v2];
    const Vertex& p3 = vertices[v3];
    bool isHit = intersection::triangleShadowHit(
        p1.position, p2.position, p3.position,
        ray, tMin, tMax, time);
    if (isHit)
        occludingShape = this;
    return isHit;
}
