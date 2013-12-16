#ifndef DW_RAYTRACER_GUI_WINDOW_H
#define DW_RAYTRACER_GUI_WINDOW_H

#include <QMainWindow>
#include "Raytracer.h"

namespace raytracer { namespace gui {

static const unsigned int WINDOW_WIDTH = 500;
static const unsigned int WINDOW_HEIGHT = 500;

class RaytracerWindow : public QMainWindow
{

	Q_OBJECT

public:
	RaytracerWindow(Raytracer* renderer);
	virtual ~RaytracerWindow();
	
	const QAction* getQuitAction() const;
	
private:
	Raytracer* renderer;
	
	QMenu* fileMenu;
		QAction* quitAction;

};

} }

#endif
