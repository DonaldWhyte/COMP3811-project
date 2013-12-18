#include <QCoreApplication>
#include <QThread>
#include <QStatusBar>

#include <sstream>
#include <iomanip> // for std::setprecision()
#include "gui/RaytracerController.h"
#include "gui/RendererWorker.h"

using namespace raytracer::gui;

RaytracerController::RaytracerController(RaytracerWindow* window, Raytracer* renderer)
	: window(window), renderer(renderer)
{
	connect(reinterpret_cast<const QObject*>(window->getQuitAction()),
		SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));
		
	CanvasWidget* canvasWidget = window->getCanvasWidget();
	Image* canvas = canvasWidget->getCanvas();
	// Create thread and move the renderer's work to it
	QThread* thread = new QThread();
	RendererWorker* worker = new RendererWorker(renderer, canvas);
	worker->moveToThread(thread);
	
	// When thread starts, start render 
	connect(thread, SIGNAL(started()), worker, SLOT(render()));
	
	// When worker has finished, display entire image on the canvas and close thread
	connect(worker, SIGNAL(finishedRow(int)), canvasWidget, SLOT(updateRowsToRender(int)));	
	connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
	// Also update raytracer window's messages
	connect(worker, SIGNAL(finishedRow(int)), this, SLOT(finishedRow(int)));	
	connect(worker, SIGNAL(finished()), this, SLOT(finished()));
	
	// Delete the worker and the thread when render has finished
	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// Connect application quite signal to thread's stop
	// TODO: clean up thread elegantly so that there is no segmentation fault
	// Start rendering!
	thread->start();
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

void RaytracerController::finished()
{
	window->statusBar()->showMessage("Raytracing has finished!");
}
