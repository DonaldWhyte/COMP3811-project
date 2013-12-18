#include <QCoreApplication>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>
#include <iomanip> // for std::setprecision() and std::fixed
#include "gui/RaytracerController.h"
#include "gui/GUICommon.h"

using namespace raytracer::gui;

RaytracerController::RaytracerController(RaytracerWindow* window, Raytracer* renderer)
	: window(window), renderer(renderer)
{		
	// TODO: clean up thread elegantly so that there is no segmentation fault
	
	// Connect window close event to controller's event handler for said event
	connect(window, SIGNAL(closed()), this, SLOT(windowClosed()));
	
	// Create renderer worker and move it to another thread
	CanvasWidget* canvasWidget = window->canvasWidget;
	Image* canvas = canvasWidget->getCanvas();
	worker = new RendererWorker(renderer, canvas);
	worker->moveToThread(&workerThread);
	// When thread starts, start render and disable save action
	connect(&workerThread, SIGNAL(started()), worker, SLOT(render()));
	// When worker has finished, display entire image on the canvas and close thread
	connect(worker, SIGNAL(finishedRow(int)), canvasWidget, SLOT(updateRowsToRender(int)));	
	// Connect start and end of thread to the event handlers in this controllers
	connect(&workerThread, SIGNAL(started()), this, SLOT(renderStarted()));
	connect(worker, SIGNAL(finished()), this, SLOT(renderFinished()));
	
	// Setup event handlers for menu bar
	connect(reinterpret_cast<const QObject*>(window->quitAction),
		SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));		
	connect(reinterpret_cast<const QObject*>(window->quitAction),
		SIGNAL(triggered()), worker, SLOT(stop()));
	connect(reinterpret_cast<const QObject*>(window->saveAction),
		SIGNAL(triggered()), this, SLOT(saveImage()));
		
	// Start timer which determines when the canvas redraws itself
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateInterface()));
	updateTimer->start(CANVAS_UPDATE_INTERVAL);	
	
	// Start rendering thread
	workerThread.start();
}

RaytracerController::~RaytracerController()
{
	delete worker;
	delete updateTimer;	
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

void RaytracerController::windowClosed()
{
	// Stop the worker and wait for the thread to finish
	worker->stop();
	workerThread.quit();
	workerThread.wait();
}
