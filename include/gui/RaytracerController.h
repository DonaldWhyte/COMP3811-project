#ifndef DW_RAYTRACER_GUI_CONTROLLER_H
#define DW_RAYTRACER_GUI_CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "gui/RaytracerWindow.h"
#include "gui/RendererWorker.h"
#include "Raytracer.h"

namespace raytracer { namespace gui {

/* How long the canvas waits before drawing itself (in milliseconds). */
static const unsigned int CANVAS_UPDATE_INTERVAL = 1000;

class RaytracerController : public QObject
{

	Q_OBJECT
	
public:
	RaytracerController(RaytracerWindow* window, Raytracer* renderer);
	virtual ~RaytracerController();

public slots:
	void renderStarted();
	void renderFinished();
	void saveImage();

	void samplingMethodChanged(int newIndex);
	void numSamplesChanged(int newValue);
	void widthChanged(int newValue);
	void heightChanged(int newValue);
	void localIlluminationChanged(int newState);
	void reflectRefractChanged(int newState);
	void shadowsChanged(int newState);
	void heightmapChanged(int newIndex);
	void viewpointChanged(int newIndex);
	void useOctreeChanged(int newState);
	void showOctreeChanged(int newState);
	void renderButtonPressed();

	/* Called periodically to ensure interface represents most recent program state. */
	void updateInterface();
	/* Called when window has been closed. */
	void windowClosed();

private:
	RaytracerWindow* window;
	Raytracer* renderer;
	
	// Thread and worker used to perform raytracing
	QThread workerThread;
	RendererWorker* worker;
	// Timer with periodically tells the canvas widget to redraw its content
	QTimer* updateTimer;
	
};

} }

#endif
