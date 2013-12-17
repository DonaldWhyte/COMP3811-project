#include <QApplication>
#include "gui/RaytracerWindow.h"
#include "gui/RaytracerController.h"

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

using namespace raytracer;

int main(int argc, char* argv[])
{
    // Seed random number generator for varying results
    srand(time(NULL));
    // Load resources
    ResourceManager* resourceManager = ResourceManager::getInstance();
    Image* terrainTextureImage = resourceManager->createImage("terrainImage", "resources/blended_terrain.tga");
    //Texture* terrainTexture = resourceManager->createTexture("terainTexture", "terrainImage");
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
    /*shapes.push_back(new Cylinder(
    	Vector3(5, 10, -30), Vector3(2, 2, 2), 2.5f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, Material::NO_REFLECTION,
        Material::NO_REFRACTION, Colour(0.0f, 0.6f, 0.0f), NULL)
    ));*/
    LineList lines;
    lines.push_back( Line(Vector3(-5, 10, -15), Vector3(-5, 10, -5)) );
    Material lineMaterial;
    lineMaterial.setColour(Colour(1.0f, 1.0f, 0.0f));
    lineMaterial.setAmbientIntensity(30.0f);
    ShapeList lineShapes = generateLines(lines, 0.25f, &lineMaterial);
    for (unsigned int i = 0; (i < lineShapes.size()); i++)
    	shapes.push_back(lineShapes[i]);

    // Load terrain heightmap
    Image* terrainHeightmap = resourceManager->createImage("heightmap", "resources/heightmap.tga");
    Vector3 terrainOffset(
        -((common::TERRAIN_CELL_SIZE * terrainHeightmap->getWidth()) / 2.0f),
        0.0f,
        -((common::TERRAIN_CELL_SIZE * terrainHeightmap->getHeight()) / 2.0f)
    );
    shapes.push_back(shapeloaders::getTerrainFromHeightmap(
        "resources/heightmap.tga", common::TERRAIN_CELL_SIZE,
        common::TERRAIN_MAX_HEIGHT, terrainOffset, terrainTexture));
    // Load skybox
    shapes.push_back(shapeloaders::getSkyBox(common::SKYBOX_SIZE, skyBoxTextures));

	// Construct raytracer to render scene
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

    // Construct QT application
	QApplication app(argc, argv);
	// Create raytracer controller and window
	gui::RaytracerWindow window(&renderer);
	gui::RaytracerController controller(&window, &renderer);
	// Show window and execute application
	window.show();
	app.exec();

	// Clean up resources
	resourceManager->clearAll();
	delete resourceManager;
	delete terrainTexture;

	return 0;
}
