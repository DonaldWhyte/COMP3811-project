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

    /* NOTE: These are inlined because the overhead caused by
     * calls to these methods was found to be a bottleneck in
     * performance during profiling. */
    inline const VertexList& getVertices() const
    {
        return vertices;
    }
    inline Material* getMaterial()
    {
        return &material;
    }
    void setVertices(const VertexList& newVertices);
    void setMaterial(const Material& newMaterial);

private:
    VertexList vertices;
    Material material;

};

#endif
