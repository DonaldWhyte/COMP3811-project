#include "DemoScene.h"

using namespace raytracer;
using namespace raytracer::gui;

DemoScene raytracer::gui::constructDemoScene()
{
    /// Load resources
    ResourceManager* resourceManager = ResourceManager::getInstance();
    Texture* terrainTexture = new TerrainHeightTexture( // multitexture for terrain
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
        Material::NO_REFRACTION, Colour(0.4f, 0.4f, 0.8f), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(-4.0f, 10.0f, -20.0f), 2.0f,
        new Material(0.5f, 3.0f, 1.0f, 20.0f, 1.0f,
        Material::NO_REFRACTION, Colour(), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(0.0f, 5.0f, -15.0f), 2.0f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, 0.5f,
            1.6666, Colour(0.8f, 0.2f, 0.2f), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(3.0f, 5.0f, -26.5f), 1.0f,
        new Material(5.0f, 0.0f, 0.0f, 0.0f, 0.4f,
        Material::NO_REFRACTION, Colour(0.9f, 0.65f, 0.0f), NULL)
    ));
    // Load skybox
    shapes.push_back(shapeloaders::getSkyBox(common::SKYBOX_SIZE, skyBoxTextures));

	// Define all possible viewpoints
	std::vector<Camera> cameras;
	cameras.reserve(3);
	cameras.push_back(Camera(
        Vector3(0, 5.0f, 0), Vector3(0, 0, -1), Vector3(0, 1, 0),
        Rect(-100, 100, -100, 100), 200, false));
	cameras.push_back(Camera(
        Vector3(-10, 3.0f, -10.0f), Vector3(1, 0.2f, -1), Vector3(0, 1, 0),
        Rect(-100, 100, -100, 100), 200, false));        
	cameras.push_back(Camera(
        Vector3(5.0f, 30.0f, -50.0f), Vector3(0.0f, -0.6f, 1), Vector3(0, 1, 0),
        Rect(-100, 100, -100, 100), 200, false));
        
    // Load all possible terrain
    std::vector<std::string> heightmapFilenames;
    heightmapFilenames.push_back("resources/heightmap.tga");
    std::vector<Image*> heightmaps;
    for (unsigned int i = 0; (i < heightmapFilenames.size()); i++)
    	heightmaps.push_back( resourceManager->createImage("heightmap", heightmapFilenames[i]) );
    std::vector<Vector3> terrainOffsets;
    for (unsigned int i = 0; (i < heightmaps.size()); i++)
    {
		terrainOffsets.push_back(Vector3(
			-((common::TERRAIN_CELL_SIZE * heightmaps[i]->getWidth()) / 2.0f),
			0.0f, -((common::TERRAIN_CELL_SIZE * heightmaps[i]->getHeight()) / 2.0f))
		);
	}
	ShapeList terrainVariants;
    for (unsigned int i = 0; (i < heightmaps.size()); i++)
    {
		Shape* unoptimisedTerrain = shapeloaders::getTerrainFromHeightmap(
			heightmapFilenames[i], common::TERRAIN_CELL_SIZE,
			common::TERRAIN_MAX_HEIGHT, terrainOffsets[i], terrainTexture, false);
		Shape* optimisedTerrain = shapeloaders::getTerrainFromHeightmap(
			heightmapFilenames[i], common::TERRAIN_CELL_SIZE,
			common::TERRAIN_MAX_HEIGHT, terrainOffsets[i], terrainTexture, true);
	    terrainVariants.push_back(unoptimisedTerrain);
        terrainVariants.push_back(optimisedTerrain);
    }
    
    // Construct test shapes
    ShapeList testShapes;
    LineList lines = dynamic_cast<Octree*>(terrainVariants[1])->getBoundingLines();
    ShapeList lineShapes = generateLines(lines, 0.3f, NULL);
    for (unsigned int i = 0; (i < lineShapes.size()); i++)
    	testShapes.push_back(lineShapes[i]);
	// Create renderer to render scene
	Raytracer* renderer = new Raytracer(cameras[0]);
    renderer->setRootShape(new BoundingShape(shapes, sceneBoundary));
    // Add light source
    renderer->addLight(PointLight(
        Vector3(-100, 70, 100),
        Colour(0.2f, 0.2f, 0.2f),
        Colour(0.4f, 0.4f, 0.4f),
        Colour(1.0f, 1.0f, 1.0f)
    ));
    renderer->addLight(PointLight(
        Vector3(3.0f, 5.0f, -26.5f),
        Colour(0.0f, 0.0f, 0.0f),
        Colour(0.6f, 0.76f, 0.0f),
        Colour(1.0f, 0.3f, 0.0f)
    ));
    // Add test shapes
    Octree* testShapeRoot = new Octree(sceneBoundary);
    for (unsigned int i = 0; (i < testShapes.size()); i++)
        testShapeRoot->insert(testShapes[i]);
    renderer->setRootTestShape(testShapeRoot);
    renderer->showTestShapes(false);
    
	// Return the entire scene
	DemoScene scene = { renderer, cameras, terrainVariants };
	return scene;
}
        
