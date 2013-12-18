#include <QApplication>
#include "gui/RaytracerWindow.h"
#include "gui/RaytracerController.h"
#include "gui/DemoScene.h"

using namespace raytracer;

int main(int argc, char* argv[])
{
    // Seed random number generator for varying results
    srand(time(NULL));

	// Create demonstration scene
	gui::DemoScene scene = gui::constructDemoScene();

    // Construct QT application
	QApplication app(argc, argv);
	// Create raytracer controller and window
	gui::RaytracerWindow window(scene.renderer);
	gui::RaytracerController controller(&window, &scene);
	// Show window and execute application
	window.show();
	app.exec();

	// Clean up resources
	delete scene.renderer;
	ResourceManager* resourceManager = ResourceManager::getInstance();
	resourceManager->clearAll();
	delete resourceManager;

	return 0;
}
