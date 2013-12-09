#include "ShapeLoaders.h"
#include "BoundingShape.h"
#include "Mesh.h"
#include "MeshTriangle.h"
#include "tiny_obj_loader.h"
#include "TGA.h"

Shape* shapeloaders::getTerrainFromHeightmap(const std::string& filename,
    float cellSize, float maxHeight)
{
    Image* heightMap = tga::readTGAFile(filename);
    if (heightMap)
    {
        int rows = heightMap->getWidth();
        int columns = heightMap->getHeight();
        // Construct mesh using heightmap's pixels as points on grid
        VertexList vertices;
        for (int x = 0; (x < columns); x++)
        {
            for (int y = 0; (y < rows); y++)
            {
                Vertex vert;
                vert.position = Vector3(x * cellSize,
                    maxHeight * heightMap->get(x, y).r,
                    y * cellSize);
                vertices.push_back(vert);
            }
        }
        delete heightMap; // no longer need height map
        Mesh* mesh = new Mesh(vertices, Material());
        // Create triangles to represent the terrain
        ShapeList triangles;
        triangles.reserve((rows - 1) * (columns - 1));
        for (int x = 0; (x < columns - 1); x++)
        {
            for (int y = 0; (y < rows - 1); y++)
            {
                int offset = x + (y * rows);
                Shape* tri1 = new MeshTriangle(mesh, offset, offset + 1, offset + 2);
                Shape* tri2 = new MeshTriangle(mesh, offset, offset + 3, offset + 1);
                triangles.push_back(tri1);
                triangles.push_back(tri2);
            }
        }

        AABB boundingBox;
        return new BoundingShape(triangles, boundingBox);
    }
    else
    {
        return NULL;
    }
}

Shape* shapeloaders::getMeshFromOBJ(const std::string& filename,
    const Vector3& translation, float scaleFactor)
{
    std::vector<tinyobj::shape_t> shapes;
    tinyobj::LoadObj(shapes, filename.c_str());
    if (shapes.size() == 0)
        return NULL;
    const tinyobj::shape_t& shape = shapes[0];

    // Find vertex positions to construct VertexList
    VertexList vertices;
    // If there's remaining values that don't form a full 3D vector, IGNORE them!
    int numVertices = shape.mesh.positions.size();
    numVertices -= (numVertices % 3);
    numVertices /= 3;
    vertices.reserve(numVertices);
    if (numVertices == 0) return NULL; // no vertices, no MESH!!!
    // We also compute the bounding box for the mesh here
    Vector3 minPosition;
    Vector3 maxPosition;
    // Do first element to initialise bounds
    Vertex firstVertex;
    firstVertex.position = Vector3(shape.mesh.positions[0],
        shape.mesh.positions[1], shape.mesh.positions[2]);
    minPosition = maxPosition = firstVertex.position;
    for (int i = 1; (i < numVertices); i++)
    {
        float transformedX = translation.x + (shape.mesh.positions[3 * i] * scaleFactor);
        float transformedY = translation.y + (shape.mesh.positions[3 * i + 1] * scaleFactor);
        float transformedZ = translation.z + (shape.mesh.positions[3 * i + 2] * scaleFactor);
        Vertex vert;
        vert.position = Vector3(transformedX, transformedY, transformedZ);
        vertices.push_back(vert);
        if (vert.position.x < minPosition.x) minPosition.x = vert.position.x;
        if (vert.position.y < minPosition.y) minPosition.y = vert.position.y;
        if (vert.position.z < minPosition.z) minPosition.z = vert.position.z;
        if (vert.position.x > maxPosition.x) maxPosition.x = vert.position.x;
        if (vert.position.y > maxPosition.y) maxPosition.y = vert.position.y;
        if (vert.position.z > maxPosition.z) maxPosition.z = vert.position.z;
    }

    // Construct material for mesh
    Material material(shape.material.ambient[0],
        shape.material.diffuse[0], shape.material.specular[0],
        shape.material.shininess, Colour(0.8f, 0.2f, 0.2f), NULL);
    // Create mesh object to assign to all triangles
    Mesh* mesh = new Mesh(vertices, material);

    // Use indices to construct a series of triangles
    ShapeList triangles;
    // If there's remaining indices that don't form a full triangle, IGNORE them!
    int numTriangles = shape.mesh.indices.size();
    numTriangles -= (numTriangles % 3);
    numTriangles /= 3;
    for (int i = 0; (i < numTriangles); i++)
    {
        Shape* tri = new MeshTriangle(mesh,
            shape.mesh.indices[3 * i],
            shape.mesh.indices[3 * i + 1],
            shape.mesh.indices[3 * i + 2]);
        triangles.push_back(tri);
    }

    // Construct bounded shape to contain all the individual triangles
    AABB boundingBox(minPosition, maxPosition);
    Shape* meshShape = new BoundingShape(triangles, boundingBox);
    return meshShape;
}
