#include <QCoreApplication>
#include <QThread>
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
	// Delete the worker and the thread when render has finished
	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// Connect application quite signal to thread's stop
	// TODO: clean up thread elegantly so that there is no segmentation fault
	// Start rendering!
	thread->start();
}
