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

static const int IMAGE_WIDTH = 4000;
static const int IMAGE_HEIGHT = 4000;

int main(int argc, char** argv)
{
    // Load resources
    Image* worldMapImage = tga::readTGAFile("resources/world_map_big.tga");
    Texture* worldMapTexture = new Texture(worldMapImage);

    // Define scene
    AABB sceneBoundary(Vector3(-100000, -100000, -100000), Vector3(100000, 100000, 100000));
    Camera camera(
        Vector3(0, 300, 0), // position
        Vector3(0, 0, -1), // direction
        //Vector3(0.4f, 0.5f, -1), // direction
        Vector3(0, 1, 0), // up
        Rectangle(-400, 400, -400, 400), // viewing rectangle
        500
    );
    ShapeList shapes;
    shapes.push_back(new Sphere(Vector3(250, 250, -1000), 150,
        new Material(0.3f, 0.9f, 0.5f, 20.0f, Colour(0.2f, 0.6f, 0.8f), worldMapTexture)
    ));
    shapes.push_back( new Triangle(
        Vector3(300, 600, -800),
        Vector3(0, 100, -1000),
        Vector3(450, 20, -1000),
        new Material(0.5f, 0.5f, 0.5f, 0.1f, Colour(0.8f, 0.2f, 0.2f), NULL)
    ));
    shapes.push_back( new Triangle(
        Vector3(0, 100, -250),
        Vector3(0, 200, -250),
        Vector3(100, 200, -250),
        new Material(0.5f, 0.5f, 0.5f, 0.1f, Colour(0.8f, 0.7f, 0.2f), NULL)
    ));
    shapes.push_back( new Triangle(
        Vector3(100, 0, -100),
        Vector3(100, 200, -100),
        Vector3(300, 200, -100),
        new Material(0.5f, 0.5f, 0.5f, 0.1f, Colour(0.2f, 0.7f, 0), NULL)
    ));
    shapes.push_back( shapeloaders::getMeshFromOBJ(
        "resources/halberd.obj", Vector3(0, 0, -1000), 20) );
    shapes.push_back( shapeloaders::getTerrainFromHeightmap(
        "resources/heightmap.tga", 100.0f, 1000.0f) );

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
    // Loop over the pixels of the image
    for (int i = 0; (i < output.getWidth()); i++)
    {
        for (int j = 0; (j < output.getHeight()); j++)
        {
            // Convert pixel coordinates (i, j) to viewing plane coordinates (x, y)
            // Note that this gets the pixel CENTRE
            float x = (static_cast<float>(i) + 0.5f) / output.getWidth(); // a
            float y = (static_cast<float>(j) + 0.5f) / output.getHeight(); // n
            bool hit = raytracer.raytrace(x, y, resultantColour);
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

    return 0;
}
