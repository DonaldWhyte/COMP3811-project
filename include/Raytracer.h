#ifndef DW_RAYTRACER_RAYTRACER_H
#define DW_RAYTRACER_RAYTRACER_H

#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Ray.h"

static const int MAX_TRACE_DEPTH = 10;
static const float MAX_RAY_DISTANCE = 100000.0f;
// Determines the contribution of reflected/refracted colours
// to the final colour of a surface.
static const float REFLECTED_REFRACTED_WEIGHT = 0.5f;
// Determines the contribution of local illumination
// to the final colour of a surface.
static const float LOCAL_ILLUMINATION_WEIGHT = (1 - REFLECTED_REFRACTED_WEIGHT);

class Raytracer
{

public:
    Raytracer(const Camera& camera);
    virtual ~Raytracer();

    /* Single and multisample raytracing. */
    bool raytrace(float x, float y, Colour& result);
    bool multisample(float x, float y, float range,
        unsigned int samples, Colour& result);
    /* Methods which compute the contribution of different physical
     * phenoma to the final pixel colour. */
    Colour localIllumination(const HitRecord& record);
    Colour reflectionAndRefraction(const Vector3& rayDirection,
        const HitRecord& record, int depth);

    /* Set a new root shape in the shape hierarchy. */
    void setRootShape(Shape* newRoot);
    /* Add another light ot the scene. */
    void addLight(const PointLight& light);
    /* Retrieve reference to scene's camera. */
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

    float computeSurfaceReflectivity(const Vector3& incoming,
        const Vector3& surfaceNormal, float originRefractiveIndex,
        float hitRefractiveIndex);
    bool computeRefractedRay(const Vector3 incidentDirection,
        const Vector3& pointOfIntersection, const Vector3& surfaceNormal,
        float refractiveIndex1, float refractiveIndex2,
        Ray& result);

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
