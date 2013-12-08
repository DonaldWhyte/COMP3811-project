#include "MeshTriangle.h"
#include "Common.h"
#include "tiny_obj_loader.h"

MeshTriangle::MeshTriangle(Mesh* mesh, int v1, int v2, int v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3)
{
}

MeshTriangle::~MeshTriangle()
{
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
        record.material = material;
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

MeshTriangleList fromOBJFile(const std::string& filename)
{
    std::vector<tinyobj::shape_t> shapes;
    tinyobj::LoadObj(shapes, filename.c_str());
    if (shapes.size() == 0)
        return MeshTriangleList();
    const tinyobj::shape_t& shape = shapes[0];

    // Find vertex positions, nornals and texture coordinates to construct VertexList
    VertexList vertices;
    int numVertices = shape.mesh.positions.size();
    numVertices -= (numVertices % 3);
    for (int i = 0; (i < numVertices); i += 3)
    {
        Vertex vert;
        vert.position = Vector3(
            shape.mesh.positions[i],
            shape.mesh.positions[i + 1],
            shape.mesh.positions[i + 2]);
        vertices.push_back(vert);
    }

    // Construct material for mesh
    Material material(shape.material.ambient[0],
        shape.material.diffuse[0], shape.material.specular[0],
        shape.material.shininess, Colour(0.8f, 0.2f, 0.2f), NULL);
    // Create mesh object to assign to all triangles
    Mesh* mesh = new Mesh(vertices, material);

    // Use indices to construct a series of triangles
    MeshTriangleList triangles;
    // If there's remaining indices that don't form a full triangle, IGNORE them!
    int numTriangles = shape.mesh.indices.size();
    numTriangles -= (numTriangles % 3);
    for (int i = 0; (i < numTriangles); i += 3)
    {
        Shape* tri = new MeshTriangle(mesh,
            shape.mesh.indices[i],
            shape.mesh.indices[i + 1],
            shape.mesh.indices[i + 2]);
        triangles.push_back(tri);
    }

    return triangles;
}
