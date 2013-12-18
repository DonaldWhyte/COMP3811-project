#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QDockWidget>
#include <QLabel>
#include "gui/RaytracerWindow.h"

using namespace raytracer::gui;

RaytracerWindow::RaytracerWindow(Raytracer* renderer) : renderer(renderer)
{
	// Set title and size of window
	setWindowTitle("COMP3811 Graphics Project -- Raytracer (Donald Whyte)");
	// Create menus
	fileMenu = menuBar()->addMenu("&File");
	saveAction = fileMenu->addAction("&Save");
	quitAction = fileMenu->addAction("&Quit");
	// Create canvas widget and add to centre of window
	canvasWidget = new CanvasWidget(CANVAS_WIDTH, CANVAS_HEIGHT);
	setCentralWidget(canvasWidget);
	
	// Create toolbox dock to the right of the canvas
	QDockWidget* toolbox = new QDockWidget("Settings");
	toolbox->setWidget(new QLabel("HELLO!"));	
	addDockWidget(Qt::RightDockWidgetArea, toolbox);
}

RaytracerWindow::~RaytracerWindow()
{
	delete canvasWidget;
	delete quitAction;
	delete saveAction;
	delete fileMenu;
}

QAction* RaytracerWindow::getSaveAction() { return saveAction; }
QAction* RaytracerWindow::getQuitAction() { return quitAction; }
CanvasWidget* RaytracerWindow::getCanvasWidget() { return canvasWidget; }
