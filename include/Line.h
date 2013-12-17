#ifndef DW_RAYTRACER_LINE_H
#define DW_RAYTRACER_LINE_H

#include "Triangle.h"
#include "Material.h"
#include "AABB.h"

/* Code for generatingtriangles that representing straight 2D lines in 3D space. */
namespace raytracer {

	struct Line
	{

		// Start adn end points of the line
		Vector3 start;
		Vector3 end;

		Line(const Vector3& start, const Vector3& end) : start(start), end(end) { }

	};

	typedef std::vector<Line> LineList;

	/* Given a list of lines, return Shape instances which can be used to render
	 * each of these lines. Each line will use the given material. */
	ShapeList generateLines(const LineList& lines, float lineThickness, Material* material);

    /* Return list of lines which represent the edges of the given bounding box. */
    LineList generateLinesFromBox(const AABB& boundingBox);

}

#endif
