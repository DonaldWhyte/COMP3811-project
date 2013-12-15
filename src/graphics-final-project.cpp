#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Image.h"
#include "Sphere.h"
#include "BoundingShape.h"
#include "ShapeLoaders.h"
#include "Camera.h"
#include "TGA.h"
#include "Raytracer.h"
#include "MeshTriangle.h"
#include "Octree.h"
#include "Common.h"

static const int IMAGE_WIDTH = 500;
static const int IMAGE_HEIGHT = 500;

static const float TERRAIN_CELL_SIZE = 10.0f;
static const float TERRAIN_MAX_HEIGHT = 75.0f;

int main(int argc, char** argv)
{
    // Seed random number generator for varying results
    srand(time(NULL));

    // Load resources
    Image* worldMapImage = tga::readTGAFile("resources/world_map.tga");
    Texture* worldMapTexture = new Texture(worldMapImage);
    Image* terrainTextureImage = tga::readTGAFile("resources/terrain.tga");
    Texture* terrainTexture = new Texture(terrainTextureImage);

    // Define scene
    AABB sceneBoundary(Vector3(-100000, -100000, -100000), Vector3(100000, 100000, 100000));
    Camera camera(
        Vector3(0, 5.0f, 0), // position
        Vector3(0, 0, -1), // direction
        //Vector3(0.4f, 0.5f, -1), // direction
        Vector3(0, 1, 0), // up
        Rectangle(-100, 100, -100, 100), // viewing rectangle
        200,
        false
    );
    ShapeList shapes;
    shapes.push_back(new Sphere(Vector3(0.0f, 7.5f, -7.5f), 2.0f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, 0.5f, 0.0f, 0.0f,
            Colour(0.2f, 0.6f, 0.8f), worldMapTexture)
    ));
    /*shapes.push_back(new Sphere(Vector3(-175, 0, 100), 40,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, 0.5f, 0.0f, 0.0f,
            Colour(0.2f, 0.6f, 0.8f), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(0, -75, 200), 40,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, 0.5f, 1.0f, 1.0f,
            Colour(0.6f, 0.2f, 0.2f), NULL)
    ));*/

    // Load terrain heightmap
    Image* terrainHeightmap = tga::readTGAFile("resources/heightmap.tga");
    Vector3 terrainOffset(
        -((TERRAIN_CELL_SIZE * terrainHeightmap->getWidth()) / 2.0f),
        0.0f,
        -((TERRAIN_CELL_SIZE * terrainHeightmap->getHeight()) / 2.0f)
    );
    shapes.push_back(shapeloaders::getTerrainFromHeightmap(
        "resources/heightmap.tga", TERRAIN_CELL_SIZE, TERRAIN_MAX_HEIGHT,
        terrainOffset, terrainTexture));


    // Create raytracer
    Raytracer raytracer(camera);
    raytracer.setRootShape(new BoundingShape(shapes, sceneBoundary));
    // Add light source
    raytracer.addLight(PointLight(
        Vector3(-300, 0, 200),
        Colour(0.2f, 0.2f, 0.2f),
        Colour(0.4f, 0.4f, 0.4f),
        Colour(1.0f, 1.0f, 1.0f)
    ));

    // Create object to store image output
    Colour backgroundColour(0.2f, 0.2f, 0.2f);
    Image output(IMAGE_WIDTH, IMAGE_HEIGHT);
    output.clear(backgroundColour);

    // Perform raytrace
    Colour resultantColour;
    float range = 1.0f / output.getWidth();
    // Loop over the pixels of the image
    for (int i = 0; (i < output.getWidth()); i++)
    {
        for (int j = 0; (j < output.getHeight()); j++)
        {
            // Convert pixel coordinates (i, j) to viewing plane coordinates (x, y)
            // Note that this gets the pixel CENTRE
            float x = (static_cast<float>(i) + 0.5f) / output.getWidth(); // a
            float y = (static_cast<float>(j) + 0.5f) / output.getHeight(); // b
            bool hit = raytracer.raytrace(x, y, resultantColour);
            //bool hit = raytracer.multisample(x, y, range, 32, resultantColour);
            // TODO: get multisampling working with non-square images! (rangeX + rangeY)
            if (hit)
                output.set(i, j, resultantColour);
            else
                output.set(i, j, backgroundColour);
        }
    }

    // Display statistics on raytracer
    std::cout << "Primary rays: " << raytracer.primaryRays() << std::endl;
    std::cout << "Reflected rays: " << raytracer.reflectedRays() << std::endl;
    std::cout << "Refracted rays: " << raytracer.refractedRays() << std::endl;
    std::cout << "Shadow rays: " << raytracer.shadowRays() << std::endl;
    std::cout << "Total rays: " << raytracer.totalRays() << std::endl;

    tga::writeTGAFile("output.tga", output);

    // Cleanup resources
    delete worldMapImage;
    delete worldMapTexture;
    delete terrainTextureImage;
    delete terrainTexture;

    return 0;
}
