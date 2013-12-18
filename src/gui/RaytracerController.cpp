#include <QCoreApplication>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>
#include <iomanip> // for std::setprecision() and std::fixed
#include "gui/RaytracerController.h"
#include "gui/GUICommon.h"
#include "Common.h"

using namespace raytracer::gui;

RaytracerController::RaytracerController(RaytracerWindow* window, Raytracer* renderer)
	: window(window), renderer(renderer), workerThread(NULL), worker(NULL)
{		
	// TODO: clean up thread elegantly so that there is no segmentation fault
	
	// Connect window close event to controller's event handler for said event
	connect(window, SIGNAL(closed()), this, SLOT(windowClosed()));
	
	// Setup event handlers for menu bar
	connect(reinterpret_cast<const QObject*>(window->quitAction),
		SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));		
	connect(reinterpret_cast<const QObject*>(window->saveAction),
		SIGNAL(triggered()), this, SLOT(saveImage()));
		
	// Event handlers for raytracer settings
	connect(window->sampMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(samplingMethodChanged(int)));
	// Event handlers for effects settings
	connect(window->localIlluminationSwitch, SIGNAL(stateChanged(int)), this, SLOT(localIlluminationChanged(int)));
	// Event handlers for geometric optimisation settings
	connect(window->useOctree, SIGNAL(stateChanged(int)), this, SLOT(useOctreeChanged(int)));
	// Event handler for render button
	connect(window->renderButton, SIGNAL(clicked()), this, SLOT(renderButtonPressed()));
		
	// Start timer which determines when the canvas redraws itself
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateInterface()));
	updateTimer->start(CANVAS_UPDATE_INTERVAL);	
	
	// Define three cameras to use for the scene
	cameras.reserve(3);
	cameras.push_back(Camera(
        Vector3(0, 5.0f, 0), Vector3(0, 0, -1), Vector3(0, 1, 0),
        Rect(-100, 100, -100, 100), 200, false));
	cameras.push_back(Camera(
        Vector3(-5, 0.0f, 0), Vector3(0.5, 1, -1), Vector3(0, 1, 0),
        Rect(-100, 100, -100, 100), 200, false));        
	cameras.push_back(Camera(
        Vector3(-10, 10.0f, 0), Vector3(0.75, -1, -1), Vector3(0, 1, 0),
        Rect(-100, 100, -100, 100), 200, false));        
	// Add cameras to viewpoint combo box
	for (unsigned int i = 0; (i < cameras.size()); i++)
	{
		std::string camName = "Camera" + common::toString(i);
		window->viewpoint->addItem( QString::fromStdString(camName) );
	}
}

RaytracerController::~RaytracerController()
{
	worker->deleteLater();
	workerThread->deleteLater();
	delete updateTimer;	
}

void RaytracerController::renderStarted()
{
	window->toolboxDock->setEnabled(false);
	window->saveAction->setEnabled(false);
}

void RaytracerController::renderFinished()
{
	window->toolboxDock->setEnabled(true);
	window->saveAction->setEnabled(true);
}

void RaytracerController::samplingMethodChanged(int newIndex)
{
	if (newIndex == 0) // if single sampling is used, disable num samples
		window->numSamples->setEnabled(false);
	else
		window->numSamples->setEnabled(true);
}

void RaytracerController::localIlluminationChanged(int newState)
{
	bool checked = (newState == Qt::Checked);
	window->shadowsSwitch->setEnabled(checked);
}

void RaytracerController::useOctreeChanged(int newState)
{
	bool checked = (newState == Qt::Checked);
	window->showOctree->setEnabled(checked);
}

void RaytracerController::renderButtonPressed()
{
	// Wait for current worker thread to finish, then create a new thread
	if (workerThread && !workerThread->isFinished())
	{
		worker->stop();
		workerThread->quit();
		workerThread->wait();
	}
	workerThread = new QThread();
	// Create renderer worker which will perform the actual rendering
	CanvasWidget* canvasWidget = window->canvasWidget;
	Image* canvas = canvasWidget->getCanvas();
	worker = new RendererWorker(renderer, canvas);
	worker->moveToThread(workerThread);
	// When thread starts, start render and disable save action
	connect(workerThread, SIGNAL(started()), worker, SLOT(render()));
	// When worker has finished, display entire image on the canvas and close thread
	connect(worker, SIGNAL(finishedRow(int)), canvasWidget, SLOT(updateRowsToRender(int)));	
	// Connect start and end of thread to the event handlers in this controllers
	connect(workerThread, SIGNAL(started()), this, SLOT(renderStarted()));
	connect(worker, SIGNAL(finished()), this, SLOT(renderFinished()));
	// If application closes, stop worker
	connect(reinterpret_cast<const QObject*>(window->quitAction),
		SIGNAL(triggered()), worker, SLOT(stop()));	

	// Assign chosen camera and heightmap
	// TODO: heightmap
	Camera* currentCamera = renderer->getCamera();
	*currentCamera = cameras[window->viewpoint->currentIndex()];
	// Configure effects settings
	bool checked = (window->localIlluminationSwitch->checkState() == Qt::Checked);
	renderer->enableLocalIllumination(checked);
 	checked = (window->reflectRefractSwitch->checkState() == Qt::Checked);
	renderer->enableReflectionAndRefraction(checked);
	checked = (window->shadowsSwitch->checkState() == Qt::Checked);
	renderer->enableShadows(checked);
	// Configure raytracer settings
	int sampleMethodIndex = window->sampMethod->currentIndex();
	worker->setSamplingMethod( static_cast<SamplingMethod>(sampleMethodIndex) );
	worker->setNumSamples( window->numSamples->value() );
	// Resize canvas to required size and clear it
	int width = window->widthBox->value();
	int height = window->heightBox->value();
	window->canvasWidget->resizeAndClear(width, height);
	
	// START RENDERING!
	workerThread->start();
}

void RaytracerController::saveImage()
{
	// Ask user where they want to save the image file
    QString filename = QFileDialog::getSaveFileName(window,
        tr("Save Image"), "./", tr("Image Files (*.png  *.jpg *.bmp)"));
    // Just do nothing if the filename is empty - user cancelled operation
    if (filename.size() == 0)
    	return;
   	// Generate QImage which can be saved
   	QImage image = toQImage(window->canvasWidget->getCanvas());
    // Saves image to the file specified
    if (!image.save(filename))
    {
        QMessageBox messageBox;
        messageBox.setText("Unknown error occured when saving image!");
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
}

void RaytracerController::updateInterface()
{
	// Redraw contents of canvas
	window->canvasWidget->update();
	
	// Total rows to render
	int totalRows = window->canvasWidget->getCanvas()->getHeight();
	// Number of rows currently renderer
	int rowsComplete = window->canvasWidget->getRowsToRender();
	
	// Construct progress message to display in status bar
	QString message;
	// If rendering has finished, then display finished message
	if (rowsComplete == totalRows)
	{
		message = "Rendering has finished!";
	}
	// If there are still rows to render, show current progress
	else
	{
		float progressPercentage = (static_cast<float>(rowsComplete) / totalRows) * 100;
		std::stringstream ss;
		ss << std::setprecision(2) << std::fixed << progressPercentage << "% complete (rendering row " << rowsComplete << " out of " << totalRows << ")";
		message = QString::fromStdString(ss.str());
	}
	window->statusBar()->showMessage(message);	
}

void RaytracerController::windowClosed()
{
	// Stop the worker and wait for the thread to finish
	worker->stop();
	workerThread->quit();
	workerThread->wait();
}
