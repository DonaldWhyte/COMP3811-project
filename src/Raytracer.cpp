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
    // Ensure recursive raytracer does not exceed maximum depth
    if (depth > MAX_TRACE_DEPTH) return false;

    bool isAHit = false;
    float tMax = MAX_RAY_DISTANCE;
    for (int i = 0; (i < shapes.size()); i++)
    {
        // Keeping tMax up-to-date ensures that that only the colour
        // of the CLOSEST point is considered at the end of the loop
        if (shapes[i]->hit(ray, 0.00001f, tMax, 0.0f, record))
        {
            // New maximum allowed distance becomes distance of this shape
            tMax = record.t;
            isAHit = true;
        }
    }

    if (isAHit)
    {
        // NOTE: At this point, the colour of the hit shape should be in the hit record
        Colour localColour, reflectedColour;

        // Add illumination to object for each light source in the scene
        for (int i = 0; (i < lights.size()); i++)
        {
            Vector3 lightDirection = (lights[i].getPosition() - record.pointOfIntersection).normalise();
            // Ambient lighting
            //localColour += (lights[i].getAmbient() * material
        }
        // TODO: lighting
        float intensity = fabs( record.normal.dot(camera.getBasisY()) );
        // Compute LOCAL colour of pixel (takes diffuse and specular into account)
        localColour = (0.1f * record.colour) + (0.9f * intensity * record.colour);

        // Handle reflection
        Ray reflectedRay = Ray(record.pointOfIntersection, record.normal);
        HitRecord reflectRecord;
        if (recursiveTrace(reflectedRay, reflectRecord, depth + 1))
            reflectedColour = reflectRecord.colour;

        // Combine computed colours into one
        record.colour = localColour + (0.5f * reflectedColour);
    }

    return isAHit;
}
