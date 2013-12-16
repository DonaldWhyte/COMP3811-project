#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include "RaytracerWindow.h"

using namespace raytracer::gui;

RaytracerWindow::RaytracerWindow(Raytracer* renderer) : renderer(renderer)
{
	// Set title and size of window
	setWindowTitle("COMP811 Graphics Project -- Raytracer (Donald Whyte)");
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// Disable resizing
	this->statusBar()->setSizeGripEnabled(false);
	// Create menus
	fileMenu = menuBar()->addMenu("&File");
	quitAction = fileMenu->addAction("&Quit");
}

RaytracerWindow::~RaytracerWindow()
{
	delete quitAction;
	delete fileMenu;
}

const QAction* RaytracerWindow::getQuitAction() const
{
	return quitAction;
}
