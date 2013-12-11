#include <iostream>
#include "Image.h"
#include "Sphere.h"
#include "Triangle.h"
#include "BoundingShape.h"
#include "ShapeLoaders.h"
#include "Camera.h"
#include "TGA.h"
#include "Raytracer.h"
#include "MeshTriangle.h"

#include "Common.h"
#include <ctime>
#include <cstdlib>

static const int IMAGE_WIDTH = 500;
static const int IMAGE_HEIGHT = 500;

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
        Vector3(0, 0, 1000), // position
        Vector3(0, 0, -1), // direction
        //Vector3(0.4f, 0.5f, -1), // direction
        Vector3(0, 1, 0), // up
        Rectangle(-1000, 1000, -1000, 1000), // viewing rectangle
        1000,
        true
    );
    ShapeList shapes;
    shapes.push_back(new Sphere(Vector3(250, 250, -1000), 150,
        new Material(0.3f, 0.9f, 0.5f, 20.0f, 0.5f,
            Colour(0.2f, 0.6f, 0.8f), worldMapTexture)
    ));
    Vector3 offset(200.0f, 0.0f, 0.0f);
    shapes.push_back( new Triangle(
        //Vector3(300, 600, -800),
        //Vector3(0, 100, -1000),
        //Vector3(450, 20, -1000),
        Vector3(300, 600, -800) + offset,
        Vector3(0, 100, -1000) + offset,
        Vector3(450, 20, -1000) + offset,
        new Material(0.5f, 0.5f, 0.5f, 0.1f, 0.1f,
            Colour(0.8f, 0.2f, 0.2f), NULL)
    ));
    shapes.push_back( new Triangle(
        Vector3(0, 100, -250),
        Vector3(0, 200, -250),
        Vector3(100, 200, -250),
        new Material(0.5f, 0.5f, 0.5f, 0.1f, 0.1f,
            Colour(0.8f, 0.7f, 0.2f), NULL)
    ));
    shapes.push_back( new Triangle(
        Vector3(100, 0, -100),
        Vector3(100, 200, -100),
        Vector3(300, 200, -100),
        new Material(0.5f, 0.5f, 0.5f, 0.1f, 0.1f,
            Colour(0.2f, 0.7f, 0), NULL)
    ));
    /* Define floor. */
    /*std::vector<Vector3> floorVerts(4);
    floorVerts[0] = Vector3(2, 1, -1) * 500.0f;
    floorVerts[1] = Vector3(2, 1, -6) * 500.0f;
    floorVerts[2] = Vector3(-3, 1, -6) * 500.0f;
    floorVerts[3] = Vector3(-3, 1, -1) * 500.0f;
    shapes.push_back( new Triangle(
        floorVerts[0], floorVerts[1], floorVerts[2],
        new Material(0.5f, 0.5f, 0.1f, 0.05f, 0.1f, Colour(1.0f, 0.0f, 0.0f), NULL)
    ));
    shapes.push_back( new Triangle(
        floorVerts[0], floorVerts[3], floorVerts[2],
        new Material(0.5f, 0.5f, 0.1f, 0.05f, 0.1f, Colour(1.0f, 0.0f, 0.0f), NULL)
    ));*/
    /*shapes.push_back( shapeloaders::getMeshFromOBJ(
        "resources/halberd.obj", Vector3(0, 0, -1000), 20) );*/
    shapes.push_back( shapeloaders::getTerrainFromHeightmap(
        "resources/heightmap.tga", 150.0f, 1000.0f,
        Vector3(0, -450, -500.0f), terrainTexture) );

    // Create raytracer
    Raytracer raytracer(camera);
    raytracer.setRootShape(new BoundingShape(shapes, sceneBoundary));
    // Add light source
    raytracer.addLight(PointLight(
        Vector3(0, 0, 0),
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
            //bool hit = raytracer.multisample(x, y, range, 8, resultantColour);
            bool hit = raytracer.raytrace(x, y, resultantColour);

            // TODO: get multisampling working with non-square images! (rangeX + rangeY)

            if (hit)
                output.set(i, j, resultantColour);
            else
                output.set(i, j, backgroundColour);
        }
    }

    tga::writeTGAFile("output.tga", output);

    // Cleanup resources
    delete worldMapImage;
    delete worldMapTexture;
    delete terrainTextureImage;
    delete terrainTexture;

    return 0;
}
