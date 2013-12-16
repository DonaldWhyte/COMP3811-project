#include "gui/RendererWorker.h"

using namespace raytracer;
using namespace gui;

static const Colour BACKGROUND_COLOUR(0.2f, 0.2f, 0.2f);

RendererWorker::RendererWorker(Raytracer* renderer, Image* canvas) :
	renderer(renderer), canvas(canvas)
{
}
	
void RendererWorker::render()
{	
    // Loop over the pixels of the image
    unsigned int canvasWidth = canvas->getWidth();
    unsigned int canvasHeight = canvas->getHeight();
    Colour resultantColour;
	for (unsigned int j = 0; (j < canvasHeight); j++)
	{
		for (unsigned int i = 0; (i < canvasWidth); i++)
        {
            // Convert pixel coordinates (i, j) to viewing plane coordinates (x, y)
            // Note that this gets the pixel CENTRE
            float x = (static_cast<float>(i) + 0.5f) / canvasWidth; // a
            float y = (static_cast<float>(j) + 0.5f) / canvasHeight; // b
            bool hit = renderer->raytrace(x, y, resultantColour);
            if (hit)
                canvas->set(i, j, resultantColour);
            else
                canvas->set(i, j, BACKGROUND_COLOUR);
        }
        emit finishedRow(j);
    }
    
	emit finished();    
}
