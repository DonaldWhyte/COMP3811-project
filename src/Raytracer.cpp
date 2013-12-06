#include "Raytracer.h"

Raytracer::Raytracer(const Camera& camera, float tMax) :
    camera(camera), initialTMax(tMax), tMax(tMax)
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
    // Reset raytracer state and construct initial hit record
    reset();
    HitRecord record;
    // Perform a recursive raytrace
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

void Raytracer::reset()
{
    tMax = initialTMax;
}

bool Raytracer::recursiveTrace(const Ray& ray, HitRecord& record, int depth)
{
    bool isAHit = false;
    for (int i = 0; (i < shapes.size()); i++)
    {
        if (shapes[i]->hit(ray, 0.00001f, tMax, 0.0f, record))
        {
            tMax = record.t;
            isAHit = true;
        }
    }
    return isAHit;
}
