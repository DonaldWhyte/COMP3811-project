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
	resize(800, 800);
	// Create menus
	fileMenu = menuBar()->addMenu("&File");
	saveAction = fileMenu->addAction("&Save");
	quitAction = fileMenu->addAction("&Quit");
	// Create canvas widget and add to centre of window
	canvasWidget = new CanvasWidget(CANVAS_WIDTH, CANVAS_HEIGHT);
	setCentralWidget(canvasWidget);
	
	// Create each section of the toolbox
	raytracerSettings = new QGroupBox("Raytracers");
	effectsSettings = new QGroupBox("Effects");
	sceneSettings = new QGroupBox("Scene");
	geometricOptSettings = new QGroupBox("Geometric Optimisation");
	
	// Create actual toolbox widget to store all settings
	toolboxLayout = new QVBoxLayout();
	toolboxLayout->addWidget(raytracerSettings);
	toolboxLayout->addWidget(effectsSettings);
	toolboxLayout->addWidget(sceneSettings);
	toolboxLayout->addWidget(geometricOptSettings);	
	// Create toolbox dock to the right of the canvas
	toolboxDock = new QDockWidget("Settings");
	toolboxWidget = new QWidget();
	toolboxWidget->setLayout(toolboxLayout);
	toolboxDock->setWidget(toolboxWidget);
	addDockWidget(Qt::RightDockWidgetArea, toolboxDock);
}

RaytracerWindow::~RaytracerWindow()
{
	delete quitAction;
	delete saveAction;
	delete fileMenu;
	
	delete canvasWidget;	
	
	delete renderButton;
	delete showOctree;
	delete useOctree;
	delete geometricOptSettings;
	delete viewpoint;
	delete viewpointLabel;
	delete sceneRowTwoLayout;
	delete terrainHeightmap;
	delete terrainLabel;
	delete sceneRowOneLayout;
	delete sceneSettings;
	delete shadowsSwitch;
	delete reflectRefractSwitch;
	delete localIlluminationSwitch;
	delete effectsSettings;
	delete heightBox;
	delete xLabel;
	delete widthBox;
	delete sizeLabel;
	delete rayRowThreeLayout;
	delete numSamples;
	delete numSamplesLabel;
	delete rayRowTwoLayout;
	delete sampMethod;
	delete sampMethodLabel;
	delete rayRowOneLayout;
	delete raytracerSettings;
	delete toolboxLayout;
	delete toolboxWidget;
	delete toolboxDock;
}

void RaytracerWindow::closeEvent(QCloseEvent* event)
{
	emit closed();
}

QAction* RaytracerWindow::getSaveAction() { return saveAction; }
QAction* RaytracerWindow::getQuitAction() { return quitAction; }
CanvasWidget* RaytracerWindow::getCanvasWidget() { return canvasWidget; }
