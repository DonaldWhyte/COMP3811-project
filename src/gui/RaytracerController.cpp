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
	// If error occurs in the worker, pipe error to this widget
	connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
	// When thread starts, start render 
	connect(thread, SIGNAL(started()), worker, SLOT(render()));
	// When worker has finished, display entire image on the canvas and close thread
	connect(worker, SIGNAL(finished()), canvasWidget, SLOT(update()));
	connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
	// Delete the worker and the thread when render has finished
	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}
