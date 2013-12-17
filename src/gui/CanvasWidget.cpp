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
	int red = col.r * 255;
	int green = col.g * 255;
	int blue = col.b * 255;
	// Ensure colours are not out of bounds
	if (red < 0) red = 0; if (red > 255) red = 255;
	if (green < 0) green = 0; if (green > 255) green = 255;
	if (blue < 0) blue = 0; if (blue > 255) blue = 255;
	return QColor(red, green, blue);
}

void CanvasWidget::updateRowsToRender(int rowIndex)
{
	rowsToRender = rowIndex + 1;
	update();
}

