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
connect(window, SIGNAL(closed()), this, SLOT(windowClosed()));

	CanvasWidget* canvasWidget = window->getCanvasWidget();
	Image* canvas = canvasWidget->getCanvas();
	
	// Create renderer worker and move it to another thread
	worker = new RendererWorker(renderer, canvas);
	worker->moveToThread(&workerThread);
	
	// When thread starts, start render and disable save action
	connect(&workerThread, SIGNAL(started()), worker, SLOT(render()));
	connect(&workerThread, SIGNAL(started()), this, SLOT(renderStarted()));
	
	// When worker has finished, display entire image on the canvas and close thread
	connect(worker, SIGNAL(finishedRow(int)), canvasWidget, SLOT(updateRowsToRender(int)));	
	// Also update raytracer window's messages
	connect(worker, SIGNAL(finishedRow(int)), this, SLOT(finishedRow(int)));
	connect(worker, SIGNAL(finished()), this, SLOT(renderFinished()));
	
	// Delete the worker and the thread when render has finished
	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
	connect(&workerThread, SIGNAL(finished()), &workerThread, SLOT(deleteLater()));
	// Connect application quit signal to thread's stop
	// TODO
	connect(QCoreApplication::instance(), SIGNAL(quit()), worker, SLOT(stop()));
	
	// Setup event handlers for menu bar
	connect(reinterpret_cast<const QObject*>(window->getQuitAction()),
		SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));		
	connect(reinterpret_cast<const QObject*>(window->getQuitAction()),
		SIGNAL(triggered()), worker, SLOT(stop()));
	connect(reinterpret_cast<const QObject*>(window->getSaveAction()),
		SIGNAL(triggered()), this, SLOT(saveImage()));
		
	// TODO: clean up thread elegantly so that there is no segmentation fault
	
	// Start rendering!
	workerThread.start();
	
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), window->getCanvasWidget(), SLOT(update()));
	updateTimer->start(CANVAS_UPDATE_INTERVAL);	
}

RaytracerController::~RaytracerController()
{
	delete worker;
	delete updateTimer;	
}

void RaytracerController::finishedRow(int rowIndex)
{
	// Construct progress messge to display in status bar
	int totalRows = window->getCanvasWidget()->getCanvas()->getHeight();
	float progressPercentage = (static_cast<float>(rowIndex) / totalRows) * 100;
	std::stringstream ss;
	ss << std::setprecision(2) << std::fixed << progressPercentage << "% complete (rendering row " << rowIndex + 1 << " out of " << totalRows << ")";
	std::string message = ss.str();
	
	window->statusBar()->showMessage( QString::fromStdString(message) );
}

void RaytracerController::renderStarted()
{
	window->getSaveAction()->setEnabled(false);
}

void RaytracerController::renderFinished()
{
	window->getSaveAction()->setEnabled(true);
	window->statusBar()->showMessage("Raytracing has finished!");
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
   	QImage image = toQImage(window->getCanvasWidget()->getCanvas());
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
	// TODO: cleanup
	std::cout << "HELLO!" << std::endl;
	worker->stop();
	workerThread.quit();
	workerThread.wait();
}
