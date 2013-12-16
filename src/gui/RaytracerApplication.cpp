#include <QApplication>
#include "gui/RaytracerWindow.h"
#include "gui/RaytracerController.h"
#include "Raytracer.h"

using namespace raytracer;

int main(int argc, char* argv[])
{
    // Seed random number generator for varying results
    srand(time(NULL));
	// Construct raytracer to render scene
	Raytracer renderer(Camera(
        Vector3(0, 5.0f, 0), // position
        Vector3(0, 0, -1), // direction
        Vector3(0, 1, 0), // up
        Rect(-100, 100, -100, 100), // viewing rectangle
        200,
        false)
    );

    // Construct QT application
	QApplication app(argc, argv);
	// Create raytracer controller and window
	gui::RaytracerWindow window(&renderer);
	gui::RaytracerController controller(&window, &renderer);
	// Show window and execute application
	window.show();
	app.exec();

	return 0;
}
