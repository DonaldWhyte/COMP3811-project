#include "RendererWorker.h"

using namespace raytracer::gui;

RendererWorker::RendererWorker(Raytracer* renderer, Image* canvas) :
	renderer(renderer), canvas(canvas)
{
}
	
void RendererWorker::render()
{
	// TODO: perform raytrace!
	emit finished();
}
