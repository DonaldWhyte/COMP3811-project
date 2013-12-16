#ifndef DW_RAYTRACER_GUI_RENDERERTHREADWORKER_H
#define DW_RAYTRACER_GUI_RENDERERTHREADWORKER_H

#include <QObject>
#include "Raytracer.h"
#include "Image.h"

namespace raytracer { namespace gui {

class RendererWorker : public QObject
{

	Q_OBJECT

public:
	RendererWorker(Raytracer* renderer, Image* canvas);
	
public slots:
	void render();
	
signals:
	void finishedRow(int rowIndex);
	void finished();
	void error(QString error);
	
private:
	Raytracer* renderer;
	Image* canvas;

};

} }

#endif
