#ifndef DW_RAYTRACER_GUICOMMON_H
#define DW_RAYTRACER_GUICOMMON_H

#include <QColor>
#include <QImage>
#include "Image.h"
#include "Colour.h"

namespace raytracer { namespace gui {

/* Convert raytracer colour to Qt representation. */
QColor toQColor(const Colour& col);
/* Convert raytracer image to QImage, an image representation
 * compatible with Qt. */
QImage toQImage(const Image* image);

} }

#endif
