#ifndef DW_RAYTRACER_GUI_CANVASWIDGET_H
#define DW_RAYTRACER_GUI_CANVASWIDGET_H

#include <QWidget>
#include "Image.h"

namespace raytracer { namespace gui {

static const unsigned int CANVAS_WIDTH = 500;
static const unsigned int CANVAS_HEIGHT = 500;

class CanvasWidget : public QWidget
{

	Q_OBJECT

public:
	CanvasWidget(unsigned int width, unsigned int height);
	
	Image* getCanvas();
	
	/* Override paint event so it draws the contents of the canvas image. */
	virtual void paintEvent(QPaintEvent* event);
	
private:
	/* Convert raytracer Colour class to QColor. */
	QColor toQColor(const Colour& col);

	Image canvas;
	
};

} }

#endif
