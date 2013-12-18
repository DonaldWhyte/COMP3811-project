#include "GUICommon.h"

using namespace raytracer;

QColor gui::toQColor(const Colour& col)
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

QImage gui::toQImage(const Image* image)
{
	unsigned int width = image->getWidth();
	unsigned int height = image->getHeight();
	QImage result(width, height, QImage::Format_RGB888);
	for (unsigned int y = 0; (y < height); y++)
	{	
		for (unsigned int x = 0; (x < width); x++)
		{
			QColor col = toQColor(image->get(x, y));
			result.setPixel(x, y, col.rgb());
		}
	}
	return result;
}
