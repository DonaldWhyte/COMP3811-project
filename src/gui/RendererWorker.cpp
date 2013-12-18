#include "gui/RendererWorker.h"

using namespace raytracer;
using namespace gui;

static const Colour BACKGROUND_COLOUR(0.2f, 0.2f, 0.2f);

RendererWorker::RendererWorker(Raytracer* renderer, Image* canvas) :
	renderer(renderer), canvas(canvas), rendering(false),
	samplingMethod(SINGLESAMPLING), numSamples(3)
{
}

void RendererWorker::setSamplingMethod(SamplingMethod newMethod)
{
	samplingMethod = newMethod;
}

void RendererWorker::setNumSamples(unsigned int newNumSamples)
{
	numSamples = newNumSamples;
}

SamplingMethod RendererWorker::getSamplingMethod() const
{
	return samplingMethod;
}

unsigned int RendererWorker::getNumSamples() const
{
	return numSamples;
}
	
void RendererWorker::render()
{	
	rendering = true;
	
    // Loop over the pixels of the image
    unsigned int canvasWidth = canvas->getWidth();
    unsigned int canvasHeight = canvas->getHeight();
    Colour resultantColour;
   
	// By defualt, render single sampled pixels
	PixelRenderingMethod renderingMethod = &RendererWorker::renderSinglesamplePixel;
	// Pick rendering method to use based on chosen sampling method
	switch (samplingMethod)
	{
	case UNIFORM_MULTISAMPLING:
		renderingMethod = &RendererWorker::renderUniformMultisamplePixel;
		break;
	case RANDOM_MULTISAMPLING:
		renderingMethod = &RendererWorker::renderUniformMultisamplePixel;
		break;
	}
	
	// Render using the chosen pixel rendering method
	for (unsigned int j = 0; (j < canvasHeight); j++)
	{
		for (unsigned int i = 0; (i < canvasWidth); i++)
        {
        	// If rendering has stopped, emit a finished signal and return from function
        	if (!rendering)
        	{
        		emit finished();
        		return;
        	}
        		
        	Colour resultantColour;
        	bool hit = ((*this).*renderingMethod)(i, j, canvasWidth, canvasHeight, resultantColour);
            if (hit)
                canvas->set(i, j, resultantColour);
            else
                canvas->set(i, j, BACKGROUND_COLOUR);
        }
        emit finishedRow(j);
    }
    
	emit finished();    
}

void RendererWorker::stop()
{
	rendering = false;
}

bool RendererWorker::renderSinglesamplePixel(int i, int j,
	unsigned int canvasWidth, unsigned int canvasHeight, Colour& result)
{
    // Convert pixel coordinates (i, j) to viewing plane coordinates (x, y)
    // Note that this gets the pixel CENTRE due to 0.5f
    float x = (static_cast<float>(i) + 0.5f) / canvasWidth; // a
    float y = (static_cast<float>(j) + 0.5f) / canvasHeight; // b
    return renderer->raytrace(x, y, result);
}

bool RendererWorker::renderUniformMultisamplePixel(int i, int j,
	unsigned int canvasWidth, unsigned int canvasHeight, Colour& result)
{
	// Define RANGE the uniformly sampled pixels will be in
    float minX = static_cast<float>(i) / canvasWidth;
    float minY = static_cast<float>(j) / canvasHeight;
    float maxX = static_cast<float>(i + 1) / canvasWidth;
    float maxY = static_cast<float>(j + 1) / canvasHeight;
    return renderer->uniformMultisample(minX, minY, maxX, maxY, numSamples, result);
}

bool RendererWorker::renderRandomMultisamplePixel(int i, int j,
	unsigned int canvasWidth, unsigned int canvasHeight, Colour& result)
{
    float minX = static_cast<float>(i) / canvasWidth;
    float minY = static_cast<float>(j) / canvasHeight;
    float maxX = static_cast<float>(i + 1) / canvasWidth;
    float maxY = static_cast<float>(j + 1) / canvasHeight;
    return renderer->randomMultisample(minX, minY, maxX, maxY, numSamples, result);
}
