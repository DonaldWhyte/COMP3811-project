#include "MeshTriangle.h"
#include "Common.h"

MeshTriangle::MeshTriangle(Mesh* mesh, int v1, int v2, int v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3)
{
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

bool MeshTriangle::hit(const Ray& ray, float tMin, float tMax, float time, HitRecord& record) const
{
    // Retrieve points from mesh
    const VertexList& vertices = mesh->getVertices();
    const Vertex& p1 = vertices[v1];
    const Vertex& p2 = vertices[v2];
    const Vertex& p3 = vertices[v3];
    // Perform ray-triangle intersection test
    bool isHit = common::triangleHit(p1, p2, p3, ray, tMin, tMax, time, record);
    if (isHit)
        record.hitShape = this;
    return isHit;
}

bool MeshTriangle::shadowHit(const Ray& ray, float tMin, float tMax, float time) const
{
    // Retrieve points from mesh
    const VertexList& vertices = mesh->getVertices();
    const Vertex& p1 = vertices[v1];
    const Vertex& p2 = vertices[v2];
    const Vertex& p3 = vertices[v3];
    return common::triangleShadowHit(
        p1.position, p2.position, p3.position,
        ray, tMin, tMax, time);
}
