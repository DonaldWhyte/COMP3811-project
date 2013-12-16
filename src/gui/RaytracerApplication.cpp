#include <QApplication>
#include "gui/RaytracerWindow.h"
#include "gui/RaytracerController.h"
#include "Raytracer.h"

#include "BoundingShape.h"
#include "Sphere.h"
#include "ShapeLoaders.h"
#include "Camera.h"
#include "TGA.h"
#include "Raytracer.h"

using namespace raytracer;

static const float TERRAIN_CELL_SIZE = 10.0f;
static const float TERRAIN_MAX_HEIGHT = 100.0f;
static const float SKYBOX_SIZE = 200.0f;

int main(int argc, char* argv[])
{
    // Seed random number generator for varying results
    srand(time(NULL));
    // Load resources
    Image* worldMapImage = tga::readTGAFile("resources/world_map.tga");
    Texture* worldMapTexture = new Texture(worldMapImage);
    Image* terrainTextureImage = tga::readTGAFile("resources/terrain.tga");
    Texture* terrainTexture = new Texture(terrainTextureImage);
    std::vector<Image*> skyBoxImages(6);
    skyBoxImages[0] = tga::readTGAFile("resources/miramar_ft.tga");
    skyBoxImages[1] = tga::readTGAFile("resources/miramar_rt.tga");
    skyBoxImages[2] = tga::readTGAFile("resources/miramar_bk.tga");
    skyBoxImages[3] = tga::readTGAFile("resources/miramar_lf.tga");
    skyBoxImages[4] = tga::readTGAFile("resources/miramar_up.tga");
    skyBoxImages[5] = tga::readTGAFile("resources/miramar_dn.tga");
    std::vector<Texture*> skyBoxTextures(6);
    for (unsigned int i = 0; (i < skyBoxTextures.size()); i++)
        if (skyBoxImages[i])
            skyBoxTextures[i] = new Texture(skyBoxImages[i]);
    // Define scene
    AABB sceneBoundary(Vector3(-10000, -10000, -10000), Vector3(10000, 10000, 10000));
    Camera camera(
        Vector3(0, 5.0f, 0), // position
        Vector3(0, 0, -1), // direction
        Vector3(0, 1, 0), // up
        Rect(-100, 100, -100, 100), // viewing rectangle
        200,
        false
    );
    ShapeList shapes;
    shapes.push_back(new Sphere(Vector3(0.0f, 8.0f, -25.0f), 2.0f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, Material::NO_REFLECTION,
        Material::NO_REFRACTION, Colour(0.2f, 0.6f, 0.8f), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(-4.0f, 10.0f, -20.0f), 1.25f,
        new Material(0.5f, 3.0f, 1.0f, 20.0f, 1.0f,
        Material::NO_REFRACTION, Colour(), NULL)
    ));
    shapes.push_back(new Sphere(Vector3(0.0f, 5.0f, -15.0f), 2.0f,
        new Material(0.5f, 1.2f, 0.5f, 20.0f, Material::NO_REFLECTION,
            1.6666, Colour(0.8f, 0.2f, 0.2f), NULL)
    ));
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
    // Load skybox
    shapes.push_back(shapeloaders::getSkyBox(SKYBOX_SIZE, skyBoxTextures));

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
	
    // Cleanup resources
    delete worldMapImage;
    delete worldMapTexture;
    delete terrainTextureImage;
    delete terrainTexture;
    for (unsigned int i = 0; (i < skyBoxImages.size()); i++)
        delete skyBoxImages[i];
    for (unsigned int i = 0; (i < skyBoxTextures.size()); i++)
        delete skyBoxTextures[i];	

	return 0;
}
