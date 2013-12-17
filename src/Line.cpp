#include "Line.h"

raytracer::ShapeList raytracer::generateLines(const raytracer::LineList& lines,
	float lineThickness, raytracer::Material* material)
{
	float halfThickness = lineThickness / 2.0f;

	ShapeList shapes;
	shapes.reserve(lines.size() * 2);
	for (unsigned int i = 0; (i < lines.size()); i++)
	{
		const Line& line = lines[i];
		Vector3 verts[4] = {
			Vector3(line.start.x, line.start.y - halfThickness, line.start.z),
			Vector3(line.start.x, line.start.y + halfThickness, line.start.z),
			Vector3(line.end.x, line.end.y - halfThickness, line.end.z),
			Vector3(line.end.x, line.end.y + halfThickness, line.end.z)
		};
			
		shapes.push_back(new Triangle(verts[0], verts[1], verts[2], material));
		shapes.push_back(new Triangle(verts[1], verts[2], verts[3], material));
	}
	return shapes;
}
