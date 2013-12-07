#ifndef DW_RAYTRACER_MESH_H
#define DW_RAYTRACER_MESH_H

#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"

struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;
};

typedef std::vector<Vertex> VertexList;

class Mesh
{

public:
    Mesh(const VertexList& vertices, const Material& material);

    const VertexList& getVertices() const;
    const Material& getMaterial() const;
    void setVertices(const VertexList& newVertices);
    void setMaterial(const Material& newMaterial);

private:
    VertexList vertices;
    Material material;

};

#endif
