#ifndef DW_RAYTRACER_GUI_CONTROLLER_H
#define DW_RAYTRACER_GUI_CONTROLLER_H

#include <QObject>
#include "gui/RaytracerWindow.h"
#include "Raytracer.h"

namespace raytracer { namespace gui {

class RaytracerController : public QObject
{

	Q_OBJECT
	
public:
	RaytracerController(RaytracerWindow* window, Raytracer* renderer);

public slots:
	void finishedRow(int rowIndex); /* called when row of image has finished rendering */
	void renderStarted();
	void renderFinished();
	void saveImage();

private:
	RaytracerWindow* window;
	Raytracer* renderer;
	
};

} }

#endif
