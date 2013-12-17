#ifndef DW_RAYTRACER_GUI_RENDERERTHREADWORKER_H
#define DW_RAYTRACER_GUI_RENDERERTHREADWORKER_H

#include <QObject>
#include "Raytracer.h"
#include "Image.h"

namespace raytracer { namespace gui {

enum SamplingMethod
{
	SINGLESAMPLING = 0,
	UNIFORM_MULTISAMPLING,
	RANDOM_MULTISAMPLING
};

class RendererWorker : public QObject
{

	Q_OBJECT

public:
	RendererWorker(Raytracer* renderer, Image* canvas);
	
	/* Getters/setters for sampling method. */
	void setSamplingMethod(SamplingMethod newMethod);
	void setNumSamples(unsigned int newNumSamples);
	/* Retrieves information on sampling method. */
	SamplingMethod getSamplingMethod() const;
	unsigned int getNumSamples() const;
	
	
public slots:
	void render();
	
signals:
	void finishedRow(int rowIndex);
	void finished();
	void error(QString error);
	
private:
	typedef bool (RendererWorker::*PixelRenderingMethod)(int, int, unsigned int, unsigned int, Colour&);

	bool renderSinglesamplePixel(int i, int j, unsigned int canvasWidth,
		unsigned int canvasHeight, Colour& result);
	bool renderUniformMultisamplePixel(int i, int j, unsigned int canvasWidth,
		unsigned int canvasHeight, Colour& result);
	bool renderRandomMultisamplePixel(int i, int j, unsigned int canvasWidth,
		unsigned int canvasHeight, Colour& result);
	
	Raytracer* renderer;
	Image* canvas;
	// Determines the sampling method used and how many samples are tkaen	
	SamplingMethod samplingMethod;
	unsigned int numSamples;

};

} }

#endif
