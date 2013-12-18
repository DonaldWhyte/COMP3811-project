#ifndef DW_RAYTRACER_GUI_CANVASWIDGET_H
#define DW_RAYTRACER_GUI_CANVASWIDGET_H

#include <QWidget>
#include "Image.h"

namespace raytracer { namespace gui {

class CanvasWidget : public QWidget
{

	Q_OBJECT

public:
	CanvasWidget(unsigned int width, unsigned int height);
	
	Image* getCanvas();
	/* Override paint event so it draws the contents of the canvas image. */
	virtual void paintEvent(QPaintEvent* event);
	
	/* Return number of rows the canvas widget should render. */
	unsigned int getRowsToRender() const;
	/* Resize the widget and its canvas to the given dimensions,
	 * clearning any content currently on the canvas. */
	void resizeAndClear(int width, int height);
	
public slots:
	/* Makes the canvas widget only render up to row with given index.
     * This should be executed when a row of the raytraced image has
     * finished rendering. */
	void updateRowsToRender(int rowIndex);
	
private:
	Image canvas;
	unsigned int rowsToRender;
	
};

} }

#endif
