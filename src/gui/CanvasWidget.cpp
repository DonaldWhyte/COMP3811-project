#include "CanvasWidget.h"
#include <QPainter>

using namespace raytracer;
using namespace gui;

CanvasWidget::CanvasWidget(unsigned int width, unsigned int height) : canvas(width, height)
{
	resize(width, height);
}
	
Image* CanvasWidget::getCanvas()
{
	return &canvas;
}

void CanvasWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	for (unsigned int x = 0; (x < canvas.getWidth()); x++)
	{
		for (unsigned int y = 0; (y < canvas.getHeight()); y++)
		{
			painter.setPen(toQColor(canvas.get(x, y)));
			painter.drawPoint(x, y);
		}
	}
}

QColor CanvasWidget::toQColor(const Colour& col)
{
	return QColor(col.r * 255, col.g * 255, col.b * 255);
}
