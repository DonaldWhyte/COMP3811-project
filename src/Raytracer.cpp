#include "Raytracer.h"
#include <cmath>
#include <iostream>

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

void Raytracer::addLight(const PointLight& light)
{
    lights.push_back(light);
}

Camera* Raytracer::getCamera()
{
    return &camera;
}

/* Help from following sources:
 * http://www.baylee-online.net/Projects/Raytracing/Algorithms/Basic-Raytrace
 * http://www.cs.jhu.edu/~cohen/RendTech99/Lectures/Ray_Tracing.bw.pdf
 * https://github.com/jelmervdl/raytracer/blob/master/scene.cpp
*/
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
        Colour localColour, reflectedColour;

        // Get hit object's material and derive source object colour from it
        Material* material = record.material;
        Colour objectColour;
        if (material->getTexture())
        {
            objectColour = material->getTexture()->getTexel(
                record.texCoord.x, record.texCoord.y);
        }
        else
        {
            objectColour = material->getColour();
        }

        // Add illumination to object for each light source in the scene
        // This is LOCAL ILLUMINATION
        for (int i = 0; (i < lights.size()); i++)
        {
            Vector3 lightDirection = (lights[i].getPosition() - record.pointOfIntersection).normalise();
            // Ambient lighting
            localColour += (lights[i].getAmbient() * objectColour * material->ambientIntensity());
            // Diffuse lighting
            float angle = lightDirection.dot(record.normal);
            if (angle > 0.0f) // only diffuse light coming from FRONT will be considered
                localColour += lights[i].getDiffuse() * objectColour * material->diffuseIntensity() * angle;
            // Specular lighting
            // Use lightDir DOT normal to compute reflection direction
            Vector3 reflectionDirection = -(lightDirection - (2.0f * angle * record.normal));
            float reflectionAngle = reflectionDirection.dot(lightDirection);
            if (reflectionAngle > 0) // only specular light from FRONT will be considered
                localColour += (lights[i].getSpecular() * material->specularIntensity()
                    * pow(reflectionAngle, material->specularExponent()));
        }

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
