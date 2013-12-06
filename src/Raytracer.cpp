#include "Raytracer.h"
#include <cmath>

Raytracer::Raytracer(const Camera& camera) : camera(camera)
{
}

Raytracer::~Raytracer()
{
    for (int i = 0; (i < shapes.size()); i++)
        delete shapes[i];
}


bool Raytracer::raytrace(float x, float y, Colour& result)
{
    // Construct Ray from Camera towards desired pixel
    Ray ray = camera.getRayToPixel(x, y, 0, 0);
    // Perform a recursive raytrace
    HitRecord record;
    bool isAHit = recursiveTrace(ray, record, 0);
    // If the ray hit an object, store resultant colour in OUT parameter
    if (isAHit)
        result = record.colour;
    return isAHit;
}

void Raytracer::addShape(Shape* shape)
{
    shapes.push_back(shape);
}

Camera* Raytracer::getCamera()
{
    return &camera;
}

bool Raytracer::recursiveTrace(const Ray& ray, HitRecord& record, int depth)
{
    bool isAHit = false;
    float tMax = MAX_RAY_DISTANCE;
    for (int i = 0; (i < shapes.size()); i++)
    {
        // Keeping tMax up-to-date ensures that that only the colour
        // of the CLOSEST point is considered at the end of the loop
        if (shapes[i]->hit(ray, 0.00001f, tMax, 0.0f, record))
        {
            // New maximum allowed distance becomes distance of this shape)
            tMax = record.t;
            isAHit = true;
        }
    }
    // NOTE: At this point, the colour of the hit shape should be in the hit record
    float intensity = fabs( record.normal.dot(camera.getBasisY()) );
    record.colour = record.colour * (0.5f * intensity);

    // TODO: lighting

    return isAHit;
}
