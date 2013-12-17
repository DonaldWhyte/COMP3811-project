#ifndef DW_RAYTRACER_GUI_CANVASWIDGET_H
#define DW_RAYTRACER_GUI_CANVASWIDGET_H

#include <QWidget>
#include "Image.h"

namespace raytracer { namespace gui {

static const unsigned int CANVAS_WIDTH = 1024;
static const unsigned int CANVAS_HEIGHT = 1024;

class CanvasWidget : public QWidget
{

	Q_OBJECT

public:
	CanvasWidget(unsigned int width, unsigned int height);
	
	Image* getCanvas();
	/* Override paint event so it draws the contents of the canvas image. */
	virtual void paintEvent(QPaintEvent* event);
	
public slots:
	/* Makes the canvas widget only render up to row with given index.
     * This should be executed when a row of the raytraced image has
     * finished rendering. */
	void updateRowsToRender(int rowIndex);
	
private:
	/* Convert raytracer Colour class to QColor. */
	QColor toQColor(const Colour& col);

	Image canvas;
	unsigned int rowsToRender;
	
};

} }

#endif
