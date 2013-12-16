#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include "RaytracerWindow.h"

using namespace raytracer::gui;

RaytracerWindow::RaytracerWindow(Raytracer* renderer) : renderer(renderer)
{
	// Set title and size of window
	setWindowTitle("COMP811 Graphics Project -- Raytracer (Donald Whyte)");
	this->setFixedSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	resize(CANVAS_WIDTH, CANVAS_HEIGHT);	
	// Disable resizing
	this->statusBar()->setSizeGripEnabled(false);
	// Create menus
	fileMenu = menuBar()->addMenu("&File");
	quitAction = fileMenu->addAction("&Quit");
	// Create canvas widget and add to centre of window
	canvasWidget = new CanvasWidget(CANVAS_WIDTH, CANVAS_HEIGHT);
	setCentralWidget(canvasWidget);
}

RaytracerWindow::~RaytracerWindow()
{
	delete canvasWidget;
	delete quitAction;
	delete fileMenu;
}

const QAction* RaytracerWindow::getQuitAction() const
{
	return quitAction;
}

CanvasWidget* RaytracerWindow::getCanvasWidget()
{
	return canvasWidget;
}
