#include "CanvasWidget.h"
#include <QPainter>
#include <algorithm>

using namespace raytracer;
using namespace gui;

CanvasWidget::CanvasWidget(unsigned int width, unsigned int height) :
	canvas(width, height), rowsToRender(0)
{
	resize(width, height);
}
	
Image* CanvasWidget::getCanvas()
{
	return &canvas;
}

void CanvasWidget::paintEvent(QPaintEvent* event)
{
	// Determine number of rows of the canvas to draw
	unsigned int canvasWidth = canvas.getWidth();
	unsigned int canvasHeight = canvas.getHeight();
	unsigned int rows = std::min(rowsToRender, canvasHeight);
	QPainter painter(this);
	// Draw spwcified amount of canvas on widget
	for (unsigned int y = 0; (y < rows); y++)
	{	
		for (unsigned int x = 0; (x < canvasWidth); x++)
		{
			painter.setPen(toQColor(canvas.get(x, y)));
			painter.drawPoint(x, y);
		}
	}
	// Fill rest of the csnvas with black
	painter.setPen(QColor(0, 0, 0));
	for (unsigned int y = rows; (y < canvasHeight); y++)
		for (unsigned int x = 0; (x < canvasWidth); x++)	
			painter.drawPoint(x, y);
}

QColor CanvasWidget::toQColor(const Colour& col)
{
	return QColor(col.r * 255, col.g * 255, col.b * 255);
}

void CanvasWidget::updateRowsToRender(int rowIndex)
{
	rowsToRender = rowIndex + 1;
	update();
}

