#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Raytracer.h"
#include "BoundingShape.h"
#include "Sphere.h"
#include "ShapeLoaders.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "TerrainHeightTexture.h"
#include "Line.h"
#include "Raytracer.h"
#include "Common.h"
#include "TGA.h"
#include "Octree.h"

using namespace raytracer;

static const int IMAGE_WIDTH = 500;
static const int IMAGE_HEIGHT = 500;

/* Cross-platform millisecond time. */
#if WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif
long getTimeInMilliseconds()
{
#if WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<long>(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
#endif
}

// Only define the main function if the Qt GUI has not been enabled
#ifndef DW_RAYTRACER_GUI_ENABLED

int main(int argc, char** argv)
{
    // Seed random number generator for varying results
    srand(time(NULL));

    // Load resources
    ResourceManager* resourceManager = ResourceManager::getInstance();
    Texture* terrainTexture = new TerrainHeightTexture(
	    resourceManager->createImage("terrainImage1", "resources/terrain_dirt.tga"),
	    resourceManager->createImage("terrainImage2", "resources/terrain_grass.tga"),
	    resourceManager->createImage("terrainImage3", "resources/terrain_rock.tga"),
	    resourceManager->createImage("terrainImage4", "resources/terrain_snow.tga")
    );

    std::vector<Image*> skyBoxImages(6);
    skyBoxImages[0] = resourceManager->createImage("skyboxFront", "resources/miramar_ft.tga");
    skyBoxImages[1] = resourceManager->createImage("skyboxRight", "resources/miramar_rt.tga");
    skyBoxImages[2] = resourceManager->createImage("skyboxBack", "resources/miramar_bk.tga");
    skyBoxImages[3] = resourceManager->createImage("skyboxLeft", "resources/miramar_lf.tga");
    skyBoxImages[4] = resourceManager->createImage("skyboxUp", "resources/miramar_up.tga");
    skyBoxImages[5] = resourceManager->createImage("skyboxDown", "resources/miramar_dn.tga");
    std::vector<Texture*> skyBoxTextures(6);
    skyBoxTextures[0] = resourceManager->createTexture("skyboxFrontTexture", "skyboxFront");
    skyBoxTextures[1] = resourceManager->createTexture("skyboxRightTexture", "skyboxRight");
	skyBoxTextures[2] = resourceManager->createTexture("skyboxBackTexture", "skyboxBack");
	skyBoxTextures[3] = resourceManager->createTexture("skyboxLeftTexture", "skyboxLeft");
	skyBoxTextures[4] = resourceManager->createTexture("skyboxUpTexture", "skyboxUp");
	skyBoxTextures[5] = resourceManager->createTexture("skyboxDownTexture", "skyboxDown");
    // Define scene
    AABB sceneBoundary(Vector3(-10000, -10000, -10000), Vector3(10000, 10000, 10000));
    ShapeList shapes;
    shapes.push_back(new Sphere(Vector3(0.0f, 8.0f, -25.0f), 2.0f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, Material::NO_REFLECTION,
        Material::NO_REFRACTION, Colour(0.2f, 0.6f, 0.8f), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(-4.0f, 10.0f, -20.0f), 2.0f,
        new Material(0.5f, 3.0f, 1.0f, 20.0f, 1.0f,
        Material::NO_REFRACTION, Colour(), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(0.0f, 5.0f, -15.0f), 2.0f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, Material::NO_REFLECTION,
            1.6666, Colour(0.8f, 0.2f, 0.2f), NULL)
    ));
    // Load terrain heightmap
    Image* terrainHeightmap = resourceManager->createImage("heightmap", "resources/heightmap.tga");
    Vector3 terrainOffset(
        -((common::TERRAIN_CELL_SIZE * terrainHeightmap->getWidth()) / 2.0f),
        0.0f,
        -((common::TERRAIN_CELL_SIZE * terrainHeightmap->getHeight()) / 2.0f)
    );
    Shape* terrain = shapeloaders::getTerrainFromHeightmap(
        "resources/heightmap.tga", common::TERRAIN_CELL_SIZE,
        common::TERRAIN_MAX_HEIGHT, terrainOffset, terrainTexture);
    shapes.push_back(terrain);
    // Load skybox
    shapes.push_back(shapeloaders::getSkyBox(common::SKYBOX_SIZE, skyBoxTextures));

    // Construct test shapes
    ShapeList testShapes;
    LineList lines = dynamic_cast<Octree*>(terrain)->getBoundingLines();
    ShapeList lineShapes = generateLines(lines, 0.3f, NULL);
    for (unsigned int i = 0; (i < lineShapes.size()); i++)
    	testShapes.push_back(lineShapes[i]);

	// Construct renderer to render scene
	Raytracer renderer(Camera(
        Vector3(0, 5.0f, 0), // position
        Vector3(0, 0, -1), // direction
        Vector3(0, 1, 0), // up
        Rect(-100, 100, -100, 100), // viewing rectangle
        200,
        false)
    );
    renderer.setRootShape(new BoundingShape(shapes, sceneBoundary));
    // Add light source
    renderer.addLight(PointLight(
        Vector3(-100, 70, 100),
        Colour(0.2f, 0.2f, 0.2f),
        Colour(0.4f, 0.4f, 0.4f),
        Colour(1.0f, 1.0f, 1.0f)
    ));
    Octree* testShapeRoot = new Octree(sceneBoundary);
    for (unsigned int i = 0; (i < testShapes.size()); i++)
        testShapeRoot->insert(testShapes[i]);
    renderer.setRootTestShape(testShapeRoot);
    renderer.showTestShapes(true);

    // Create object to store image output
    Colour backgroundColour(0.2f, 0.2f, 0.2f);
    Image output(IMAGE_WIDTH, IMAGE_HEIGHT);
    output.clear(backgroundColour);
    // Perform raytrace
    Colour resultantColour;

    long startTime = getTimeInMilliseconds();
    // Loop over the pixels of the image
    for (int i = 0; (i < output.getWidth()); i++)
    {
        for (int j = 0; (j < output.getHeight()); j++)
        {
            // Convert pixel coordinates (i, j) to viewing plane coordinates (x, y)
            // Note that this gets the pixel CENTRE
            float x = (static_cast<float>(i) + 0.5f) / output.getWidth(); // a
            float y = (static_cast<float>(j) + 0.5f) / output.getHeight(); // b
            bool hit = renderer.raytrace(x, y, resultantColour);
            if (hit)
                output.set(i, j, resultantColour);
            else
                output.set(i, j, backgroundColour);
        }
    }

    // Compute time the raytrace took
    double elapsedTime = getTimeInMilliseconds() - startTime;
    elapsedTime /= 1000.0f; // convert milliseconds to seconds
    // Display statistics on renderer
    std::cout << "Primary rays: " << renderer.primaryRays() << std::endl;
    std::cout << "Reflected rays: " << renderer.reflectedRays() << std::endl;
    std::cout << "Refracted rays: " << renderer.refractedRays() << std::endl;
    std::cout << "Shadow rays: " << renderer.shadowRays() << std::endl;
    std::cout << "Total rays: " << renderer.totalRays() << std::endl;
    std::cout << "Time taken: " << elapsedTime << " seconds" << std::endl;

    tga::writeTGAFile("output.tga", output);

    // Cleanup resources
    delete resourceManager;
    delete terrainTexture;

    return 0;
}

#endif
