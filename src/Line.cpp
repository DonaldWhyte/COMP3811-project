#include "Line.h"

raytracer::ShapeList raytracer::generateLines(const raytracer::LineList& lines,
	float lineThickness, raytracer::Material* material)
{
    // Half thickness of line for point computation later
	float halfThickness = lineThickness / 2.0f;

	ShapeList shapes;
	shapes.reserve(lines.size() * 2);
	for (unsigned int i = 0; (i < lines.size()); i++)
	{
		const Line& line = lines[i];
        // Check if line is purely vertical. If so, then line thickness is applied to the X axis
        float xThickness = 0.0f;
        float yThickness = 0.0f;
        if (line.start.x == line.end.x && line.start.z == line.end.z)
            xThickness = halfThickness;
        else
            yThickness = halfThickness;

		Vector3 verts[4] = {
			Vector3(line.start.x - xThickness, line.start.y - yThickness, line.start.z),
			Vector3(line.start.x + xThickness, line.start.y + yThickness, line.start.z),
			Vector3(line.end.x - xThickness, line.end.y - yThickness, line.end.z),
			Vector3(line.end.x + xThickness, line.end.y + yThickness, line.end.z)
		};

		shapes.push_back(new Triangle(verts[0], verts[1], verts[2], material));
		shapes.push_back(new Triangle(verts[1], verts[2], verts[3], material));
	}
	return shapes;
}

raytracer::LineList raytracer::generateLinesFromBox(const raytracer::AABB& boundingBox)
{
    const Vector3& min = boundingBox.bounds[0];
    const Vector3& max = boundingBox.bounds[1];
    // Generate the eight points that make up the bounding box
    Vector3 points[] = {
        Vector3(min.x, max.y, min.z), Vector3(min.x, max.y, max.z),
        Vector3(max.x, max.y, max.z), Vector3(max.x, max.y, min.z),
        Vector3(min.x, min.y, min.z), Vector3(min.x, min.y, max.z),
        Vector3(max.x, min.y, max.z), Vector3(max.x, min.y, min.z),
    };
    // Use those points to generate the 12 edges of the box
    LineList lines;
    lines.push_back( Line(points[0], points[1]) ); lines.push_back( Line(points[1], points[2]) );
    lines.push_back( Line(points[2], points[3]) ); lines.push_back( Line(points[3], points[0]) );
    lines.push_back( Line(points[4], points[5]) ); lines.push_back( Line(points[5], points[6]) );
    lines.push_back( Line(points[6], points[7]) ); lines.push_back( Line(points[7], points[4]) );
    lines.push_back( Line(points[0], points[4]) ); lines.push_back( Line(points[1], points[5]) );
    lines.push_back( Line(points[2], points[6]) ); lines.push_back( Line(points[3], points[7]) );
    return lines;
}
