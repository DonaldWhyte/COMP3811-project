#ifndef DW_RAYTRACER_GUI_WINDOW_H
#define DW_RAYTRACER_GUI_WINDOW_H

#include <QMainWindow>
#include "Raytracer.h"
#include "gui/CanvasWidget.h"

namespace raytracer { namespace gui {

class RaytracerWindow : public QMainWindow
{

	Q_OBJECT

public:
	RaytracerWindow(Raytracer* renderer);
	virtual ~RaytracerWindow();
	
	const QAction* getQuitAction() const;
	CanvasWidget* getCanvasWidget();
	
private:
	Raytracer* renderer;
	
	QMenu* fileMenu;
		QAction* quitAction;
	CanvasWidget* canvasWidget;

};

} }

#endif
