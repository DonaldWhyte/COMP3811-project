#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const Material& material) :
    vertices(vertices), material(material)
{
}

void Mesh::setVertices(const VertexList& newVertices)
{
    vertices = newVertices;
}

void Mesh::setMaterial(const Material& newMaterial)
{
    material = newMaterial;
}
