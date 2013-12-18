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
	resize(800, 600);
	// Create menus
	fileMenu = menuBar()->addMenu("&File");
	saveAction = fileMenu->addAction("&Save");
	quitAction = fileMenu->addAction("&Quit");
	// Create canvas widget and add to centre of window
	canvasWidget = new CanvasWidget(CANVAS_WIDTH, CANVAS_HEIGHT);
	setCentralWidget(canvasWidget);
	
	// Create each section of the toolbox
	raytracerSettings = new QGroupBox("Raytracers");
		sampMethodLabel = new QLabel("Sampling Method");
		sampMethod = new QComboBox();
		sampMethod->addItem("Single Sample");
		sampMethod->addItem("Uniform Multisampling");
		sampMethod->addItem("Random Multisampling");
		rayRowOneLayout = new QHBoxLayout();		
		rayRowOneLayout->addWidget(sampMethodLabel);
		rayRowOneLayout->addWidget(sampMethod);
		numSamplesLabel = new QLabel("# Samples");
		numSamples = new QSpinBox();
		numSamples->setRange(1, 10);
		numSamples->setValue(1);
		rayRowTwoLayout = new QHBoxLayout();
		rayRowTwoLayout->addWidget(numSamplesLabel);
		rayRowTwoLayout->addWidget(numSamples);
		sizeLabel = new QLabel("Size");	
		widthBox = new QSpinBox();
		widthBox->setRange(50, 4000);
		numSamples->setValue(500);
		xLabel = new QLabel("x");
		heightBox = new QSpinBox();
		heightBox->setRange(50, 4000);
		numSamples->setValue(500);
		rayRowThreeLayout = new QHBoxLayout();	
		rayRowThreeLayout->addWidget(sizeLabel);
		rayRowThreeLayout->addWidget(widthBox);
		rayRowThreeLayout->addWidget(xLabel);
		rayRowThreeLayout->addWidget(heightBox);
		raytracerSettingsLayout = new QVBoxLayout();
		raytracerSettingsLayout->addLayout(rayRowOneLayout);
		raytracerSettingsLayout->addLayout(rayRowTwoLayout);
		raytracerSettingsLayout->addLayout(rayRowThreeLayout);		
		raytracerSettings->setLayout(raytracerSettingsLayout);
	effectsSettings = new QGroupBox("Effects");
		localIlluminationSwitch = new QCheckBox("Local Illumination");
		reflectRefractSwitch = new QCheckBox("Reflection/Refraction");
		shadowsSwitch = new QCheckBox("Shadows");
		effectsSettingsLayout = new QVBoxLayout();
		effectsSettingsLayout->addWidget(localIlluminationSwitch);
		effectsSettingsLayout->addWidget(reflectRefractSwitch);
		effectsSettingsLayout->addWidget(shadowsSwitch);
		effectsSettings->setLayout(effectsSettingsLayout);
	sceneSettings = new QGroupBox("Scene");
		terrainLabel = new QLabel("Terrain Heightmap Size");
		terrainHeightmap = new QComboBox();
		terrainHeightmap->addItem("Small");
		terrainHeightmap->addItem("Medium");
		terrainHeightmap->addItem("Large");
		sceneRowOneLayout = new QHBoxLayout();
		sceneRowOneLayout->addWidget(terrainLabel);
		sceneRowOneLayout->addWidget(terrainHeightmap);		
		viewpointLabel = new QLabel("Viewpoint");
		viewpoint = new QComboBox();
		viewpoint->addItem("One");
		viewpoint->addItem("Two");
		viewpoint->addItem("Three");		
		sceneRowTwoLayout = new QHBoxLayout();
		sceneRowTwoLayout->addWidget(viewpointLabel);
		sceneRowTwoLayout->addWidget(viewpoint);			
		sceneSettingsLayout = new QVBoxLayout();
		sceneSettingsLayout->addLayout(sceneRowOneLayout);
		sceneSettingsLayout->addLayout(sceneRowTwoLayout);
		sceneSettings->setLayout(sceneSettingsLayout);
	geometricOptSettings = new QGroupBox("Geometric Optimisation");
		useOctree = new QCheckBox("Enable Octree");
		showOctree = new QCheckBox("Show Octree");
		geometricOptSettingsLayout = new QVBoxLayout();
		geometricOptSettingsLayout->addWidget(useOctree);
		geometricOptSettingsLayout->addWidget(showOctree);
		geometricOptSettings->setLayout(geometricOptSettingsLayout);
	renderButton = new QPushButton("RENDER");
	// Create actual toolbox widget to store all settings
	toolboxLayout = new QVBoxLayout();
	toolboxLayout->addWidget(raytracerSettings);
	toolboxLayout->addWidget(effectsSettings);
	toolboxLayout->addWidget(sceneSettings);
	toolboxLayout->addWidget(geometricOptSettings);	
	toolboxLayout->addWidget(renderButton);	
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
	delete geometricOptSettingsLayout;
	delete geometricOptSettings;
	delete viewpoint;
	delete viewpointLabel;
	delete sceneRowTwoLayout;
	delete terrainHeightmap;
	delete terrainLabel;
	delete sceneRowOneLayout;
	delete sceneSettingsLayout;
	delete sceneSettings;
	delete shadowsSwitch;
	delete reflectRefractSwitch;
	delete localIlluminationSwitch;
	delete effectsSettingsLayout;
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
	delete raytracerSettingsLayout;
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
