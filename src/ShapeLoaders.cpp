#include <sstream>
#include <float.h>
#include "ShapeLoaders.h"
#include "BoundingShape.h"
#include "Octree.h"
#include "Mesh.h"
#include "MeshTriangle.h"
#include "TGA.h"
#include "ResourceManager.h"

using namespace raytracer;

/* Reflectivity of materials loaded from Wavefront OBJ meshes. */
static const float MATERIAL_REFLECTIVITY = 0.1f;
/* Transparency and refractive index of materials loaded from Wavefront OBJ meshes. */
static const float MATERIAL_REFRACTIVE_INDEX = 0.0f;

/* Generate unique ID for mesh. */
std::string generateMeshID()
{
	static unsigned int idCount = 0;
	idCount++;
	
	std::stringstream ss;
	ss << "generated-mesh-" << idCount;
	return ss.str();
}

/* Compute height at given point on terrain grid using heightmap. */
float getHeight(Image* image, int x, int y, float maxHeight)
{
    return image->get(x, y).r * maxHeight;
}

Shape* shapeloaders::getTerrainFromHeightmap(const std::string& filename,
    float cellSize, float maxHeight, const Vector3& offset, Texture* texture)
{
    Image* heightMap = tga::readTGAFile(filename);
    if (heightMap)
    {
        int width = heightMap->getWidth();
        int height = heightMap->getHeight();
        // Keep track of minimum and maximum vertex positions for heightmap's bounding box
        Vector3 minPoint(FLT_MIN, FLT_MIN, FLT_MIN);
        Vector3 maxPoint(FLT_MAX, FLT_MAX, FLT_MAX);
        // Construct mesh using heightmap's pixels as points on grid
        VertexList vertices;
        vertices.reserve(width * height);
        for (int y = 0; (y < height); y++)
        {
            for (int x = 0; (x < width); x++)
            {
                Vertex vert;
                // Compute position of next point in terrain grid
                float pointHeight = getHeight(heightMap, x, y, maxHeight);
                vert.position = Vector3(x * cellSize, pointHeight, y * cellSize);
                vert.position += offset;
                // Update min and max points
                if (vert.position.x < minPoint.x) minPoint.x = vert.position.x;
                if (vert.position.y < minPoint.y) minPoint.y = vert.position.y;
                if (vert.position.z < minPoint.z) minPoint.z = vert.position.z;
                if (vert.position.x > maxPoint.x) maxPoint.x = vert.position.x;
                if (vert.position.y > maxPoint.y) maxPoint.y = vert.position.y;
                if (vert.position.z > maxPoint.z) maxPoint.z = vert.position.z;

                // Vertices in mesh grid alternate having 0 and 1 for tex coords
                float texX = ((x % 2) == 0) ? 0.0f : 1.0f;
                float texY = ((y % 2) == 0) ? 0.0f : 1.0f;
                vert.texCoord = Vector2(texX, texY);
                // Compute normal vector using central differencing
                float hLeft, hRight, hDown, hUp;
                if (x > 0) hLeft = getHeight(heightMap, x - 1, y, maxHeight);
                else hLeft = pointHeight;
                if (x < (width - 1)) hRight = getHeight(heightMap, x + 1, y, maxHeight);
                else hRight = pointHeight;
                if (y > 0) hDown = getHeight(heightMap, x, y - 1, maxHeight);
                else hDown = pointHeight;
                if (y < (height - 1)) hUp = getHeight(heightMap, x, y + 1, maxHeight);
                else hUp = pointHeight;
                vert.normal = Vector3((hLeft - hRight), (hDown - hUp), 2.0f).normalise();

                vertices.push_back(vert);
            }
        }
        delete heightMap; // no longer need height map

        // Material only has ambient and diffuse (no specular or reflection!)
        Material material(2.0f, 0.6f, 0.0f, 0.0f, Material::NO_REFLECTION,
            Material::NO_REFRACTION, Colour(0.2f, 0.7f, 0.2f), texture);
        // Construct mesh to hold the terrain's vertices
        ResourceManager* resourceManager = ResourceManager::getInstance();
        Mesh* mesh = resourceManager->createMesh(generateMeshID(), vertices, material);
        // Create triangles to represent the terrain
        ShapeList triangles;
        triangles.reserve((width - 1) * (height - 1) * 2);
        for (int x = 0; (x < width - 1); x++)
        {
            for (int y = 0; (y < height - 1); y++)
            {
                int offset = (y * width) + x;
                Shape* tri1 = new MeshTriangle(mesh, offset, offset + height, offset + 1);
                Shape* tri2 = new MeshTriangle(mesh, offset + height, offset + height + 1, offset + 1);
                triangles.push_back(tri1);
                triangles.push_back(tri2);
            }
        }

        AABB boundingBox(minPoint, maxPoint);
        Octree* octree = new Octree(boundingBox);
        for (unsigned int i = 0; (i < triangles.size()); i++)
            octree->insert(triangles[i]);
        return octree;
    }
    else
    {
        return NULL;
    }
}

/* Function here purely to make getSkyBox() code more readable. */
void addVertexToList(VertexList& vertices, const Vector3& position, const Vector2& texCoord)
{
    Vertex vert;
    vert.position = position;
    vert.texCoord = texCoord;
    vertices.push_back(vert);
}

Shape* shapeloaders::getSkyBox(float size, const std::vector<Texture*>& skyBoxTextures)
{
    if (skyBoxTextures.size() != 6)
    {
        std::cerr << "Exactly six textures are required to construct sky boxes ("
            << skyBoxTextures.size() << " given)" << std::endl;
        return NULL;
    }

    // Half the size so we get a sky box centered at origin
    size /= 2.0f;
    // Construct vertices for mesh
    VertexList vertices;
    vertices.reserve(6 * 4); // four vertices for each face of cube
    addVertexToList(vertices, Vector3(size, -size, -size), Vector2(0, 0)); // front
    addVertexToList(vertices, Vector3(-size, -size, -size), Vector2(1, 0));
    addVertexToList(vertices, Vector3(-size, size, -size), Vector2(1, 1));
    addVertexToList(vertices, Vector3(size, size, -size), Vector2(0, 1));

    addVertexToList(vertices, Vector3(size, -size, size), Vector2(0, 0)); // left
    addVertexToList(vertices, Vector3(size, -size, -size), Vector2(1, 0));
    addVertexToList(vertices, Vector3(size, size, -size), Vector2(1, 1));
    addVertexToList(vertices, Vector3(size, size, size), Vector2(0, 1));

    addVertexToList(vertices, Vector3(-size, -size, size), Vector2(0, 0)); // back
    addVertexToList(vertices, Vector3(size, -size, size), Vector2(1, 0));
    addVertexToList(vertices, Vector3(size, size, size), Vector2(1, 1));
    addVertexToList(vertices, Vector3(-size, size, size), Vector2(0, 1));

    addVertexToList(vertices, Vector3(-size, -size, -size), Vector2(0, 0)); // right
    addVertexToList(vertices, Vector3(-size, -size, size), Vector2(1, 0));
    addVertexToList(vertices, Vector3(-size, size, size), Vector2(1, 1));
    addVertexToList(vertices, Vector3(-size, size, -size), Vector2(0, 1));

    addVertexToList(vertices, Vector3(-size, size, -size), Vector2(0, 0)); // top
    addVertexToList(vertices, Vector3(-size, size, size), Vector2(1, 0));
    addVertexToList(vertices, Vector3(size, size, size), Vector2(1, 1));
    addVertexToList(vertices, Vector3(size, size, -size), Vector2(0, 1));

    addVertexToList(vertices, Vector3(-size, -size, -size), Vector2(0, 0)); // bottom
    addVertexToList(vertices, Vector3(-size, -size, size), Vector2(1, 0));
    addVertexToList(vertices, Vector3(size, -size, size), Vector2(1, 1));
    addVertexToList(vertices, Vector3(size, -size, -size), Vector2(0, 1));

    // Specify material (only ambient contribution with no reflection/refraction!)
    Material material(10.0f, 0.0f, 0.0f, 0.0f,
        Material::NO_REFLECTION, Material::NO_REFRACTION,
        Colour(), NULL);
    // Construct a mesh for each face of the sky box
    ResourceManager* resourceManager = ResourceManager::getInstance();
    std::vector<Mesh*> faceMeshes(6);
    for (unsigned int i = 0; (i < faceMeshes.size()); i++)
    {
        VertexList::const_iterator start = vertices.begin() + (i * 4);
        VertexList::const_iterator end = start + 4;
        Material faceMaterial = material;
        faceMaterial.setTexture(skyBoxTextures[i]);
		faceMeshes[i] = resourceManager->createMesh(generateMeshID(),
			VertexList(start, end), faceMaterial);  
    }

    std::vector<Shape*> triangles(6 * 2); // two triangles for each of the six faces
    for (unsigned int i = 0; (i < 6); i++)
    {
        unsigned int triIndex = i * 2;
        triangles[triIndex] = new MeshTriangle(faceMeshes[i], 0, 1, 2);
        triangles[triIndex + 1] = new MeshTriangle(faceMeshes[i], 0, 2, 3);
    }

    // Construct bounding box for sky box and returned shape with bounded triangles
    AABB boundingBox(Vector3(-size, -size, -size), Vector3(size, size, size));
    return new BoundingShape(triangles, boundingBox);
}
