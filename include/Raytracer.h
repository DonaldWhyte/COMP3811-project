#ifndef DW_RAYTRACER_RAYTRACER_H
#define DW_RAYTRACER_RAYTRACER_H

#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Ray.h"

static const int MAX_TRACE_DEPTH = 10;
static const float MAX_RAY_DISTANCE = 100000.0f;
// Refractive index of air.
// (source: http://en.wikipedia.org/wiki/Refractive_index)
static const float AIR_REFRACTIVE_INDEX = 1.000293;

class Raytracer
{

public:
    Raytracer(const Camera& camera);
    virtual ~Raytracer();

    bool raytrace(float x, float y, Colour& result);
    bool multisample(float x, float y, float range,
        unsigned int samples, Colour& result);

    void setRootShape(Shape* newRoot);
    void addLight(const PointLight& light);

    Camera* getCamera();

    /* Accessors for statistics on trace. */
    unsigned int primaryRays() const;
    unsigned int reflectedRays() const;
    unsigned int refractedRays() const;
    unsigned int shadowRays() const;
    unsigned int totalRays() const;
    /* Used to reset ray counts to zero. */
    void resetRayCount();

private:
    bool recursiveTrace(const Ray& ray, HitRecord& record, int depth);

    Ray computeRefractedRay(const Vector3 incidentDirection,
        const Vector3& pointOfIntersection, const Vector3& surfaceNormal,
        float surfaceRefractiveIndex);

    Shape* rootShape;
    LightList lights;
    Camera camera;

    Material defaultMaterial;

    unsigned int numPrimaryRays;
    unsigned int numReflectedRays;
    unsigned int numRefractedRays;
    unsigned int numShadowRays;

};

#endif
