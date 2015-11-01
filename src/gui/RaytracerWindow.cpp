#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include "gui/RaytracerWindow.h"

using namespace raytracer::gui;

RaytracerWindow::RaytracerWindow(Raytracer* renderer) : renderer(renderer)
{
	// Set title and size of window
	setWindowTitle("COMP3811 Graphics Project -- Raytracer (Donald Whyte)");
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// Create menus
	fileMenu = menuBar()->addMenu("&File");
	saveAction = fileMenu->addAction("&Save Image");
	quitAction = fileMenu->addAction("&Quit");
	// Create canvas widget and add to centre of window
	// Note that it is wrapped in a scroll pane in case
	// it's larger than the window
	canvasWidget = new CanvasWidget(INITIAL_CANVAS_WIDTH, INITIAL_CANVAS_HEIGHT);
	canvasScrollArea = new QScrollArea();
	canvasScrollArea->setWidget(canvasWidget);
 	canvasScrollArea->setWidgetResizable(false);
	setCentralWidget(canvasScrollArea);

	// Create each section of the toolbox
	raytracerSettings = new QGroupBox("Raytracer");
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
		numSamples->setRange(2, 10);
		numSamples->setValue(2);
		numSamples->setEnabled(false);
		rayRowTwoLayout = new QHBoxLayout();
		rayRowTwoLayout->addWidget(numSamplesLabel);
		rayRowTwoLayout->addWidget(numSamples);
		sizeLabel = new QLabel("Size");
		widthBox = new QSpinBox();
		widthBox->setRange(50, 4000);
		widthBox->setValue(1000);
		xLabel = new QLabel("x");
		heightBox = new QSpinBox();
		heightBox->setRange(50, 4000);
		heightBox->setValue(1000);
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
		localIlluminationSwitch->setChecked(true);
		reflectRefractSwitch = new QCheckBox("Reflection/Refraction");
		reflectRefractSwitch->setChecked(true);
		shadowsSwitch = new QCheckBox("Shadows");
		shadowsSwitch->setChecked(true);
		effectsSettingsLayout = new QVBoxLayout();
		effectsSettingsLayout->addWidget(localIlluminationSwitch);
		effectsSettingsLayout->addWidget(reflectRefractSwitch);
		effectsSettingsLayout->addWidget(shadowsSwitch);
		effectsSettings->setLayout(effectsSettingsLayout);
	sceneSettings = new QGroupBox("Scene");
		lightOneSwitch = new QCheckBox("Sky Light Enabled");
		lightOneSwitch->setCheckState(Qt::Checked);
		lightTwoSwitch = new QCheckBox("Yellow Light Enabled");
		lightTwoSwitch->setCheckState(Qt::Checked);
		terrainLabel = new QLabel("Terrain");
		terrainHeightmap = new QComboBox();
		sceneRowOneLayout = new QHBoxLayout();
		sceneRowOneLayout->addWidget(terrainLabel);
		sceneRowOneLayout->addWidget(terrainHeightmap);
		viewpointLabel = new QLabel("Viewpoint");
		viewpoint = new QComboBox();
		sceneRowTwoLayout = new QHBoxLayout();
		sceneRowTwoLayout->addWidget(viewpointLabel);
		sceneRowTwoLayout->addWidget(viewpoint);
		sceneSettingsLayout = new QVBoxLayout();
		sceneSettingsLayout->addWidget(lightOneSwitch);
		sceneSettingsLayout->addWidget(lightTwoSwitch);
		sceneSettingsLayout->addLayout(sceneRowOneLayout);
		sceneSettingsLayout->addLayout(sceneRowTwoLayout);
		sceneSettings->setLayout(sceneSettingsLayout);
	geometricOptSettings = new QGroupBox("Geometric Optimisation");
		useOctree = new QCheckBox("Enable Octree");
		useOctree->setChecked(true);
		showOctree = new QCheckBox("Show Octree (very slow)");
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
	toolboxDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	toolboxWidget = new QWidget();
	toolboxWidget->setLayout(toolboxLayout);
	toolboxDock->setWidget(toolboxWidget);
	addDockWidget(Qt::RightDockWidgetArea, toolboxDock);

	// Show empty message so message bar at the bottom appears straight away
	statusBar()->showMessage("");
}

RaytracerWindow::~RaytracerWindow()
{
	delete quitAction;
	delete saveAction;
	delete fileMenu;

	delete canvasWidget;
	delete canvasScrollArea;

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
	delete lightOneSwitch;
	delete lightTwoSwitch;
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

void RaytracerWindow::closeEvent(QCloseEvent* /*event*/)
{
	emit closed();
}
