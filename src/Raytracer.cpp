#include "Raytracer.h"
#include "Common.h"
#include <cmath>
#include <algorithm>

Raytracer::Raytracer(const Camera& camera) : rootShape(NULL), camera(camera)
{
}

Raytracer::~Raytracer()
{
    delete rootShape;
}

bool Raytracer::raytrace(float x, float y, Colour& result)
{
    // Construct Ray from Camera towards desired pixel
    Ray ray = camera.getRayToPixel(x, y);
    // Perform a recursive raytrace
    HitRecord record;
    bool isAHit = recursiveTrace(ray, record, 0);
    // If the ray hit an object, store resultant colour in OUT parameter
    if (isAHit)
        result = record.colour;
    return isAHit;
}

bool Raytracer::multisample(float x, float y, float range, unsigned int samples, Colour& result)
{
    Ray ray = camera.getRayToPixel(x, y);
    Colour sum;
    int hits = 0;

    for (unsigned int i = 0; (i < samples); i++)
    {
        Ray sampleRay(ray.origin(), ray.direction() + (common::monteCarloDirection() * range));
        HitRecord record;
        bool isHit = recursiveTrace(sampleRay, record, 0);
        if (isHit)
        {
            sum += record.colour;
            hits += 1;
        }
    }

    result = sum / std::max(1, hits);
    return (hits > 0);
}

void Raytracer::setRootShape(Shape* newRoot)
{
    delete rootShape; // delete the old root shape!
    rootShape = newRoot;
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
    if (!rootShape) return false;

    bool isAHit = rootShape->hit(ray, 0.00001f, MAX_RAY_DISTANCE, 0.0f, record);
    if (isAHit)
    {
        Colour localColour, reflectedColour;

        // Get hit object's material and derive source object colour from it
        Material* material = record.material;
        Colour objectColour;
        if (material)
        {
            if (material->getTexture())
            {
                objectColour = material->getTexture()->getTexel(
                    record.texCoord.x, record.texCoord.y);
            }
            else
                objectColour = material->getColour();
        }
        else
        {
            material = &defaultMaterial;
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
        // (but only if material of hit shape is actually reflective!)
        if (material->reflectivity() > 0.0f)
        {
            Ray reflectedRay = Ray(record.pointOfIntersection, record.normal);
            HitRecord reflectRecord;
            if (recursiveTrace(reflectedRay, reflectRecord, depth + 1))
                reflectedColour = reflectRecord.colour;
        }

        // TODO: transmission?

        // Combine computed colours into one
        record.colour = localColour + (material->reflectivity() * reflectedColour);
    }

    return isAHit;
}
