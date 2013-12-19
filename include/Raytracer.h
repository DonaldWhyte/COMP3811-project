#ifndef DW_RAYTRACER_RAYTRACER_H
#define DW_RAYTRACER_RAYTRACER_H

#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Ray.h"

namespace raytracer {

static const int MAX_TRACE_DEPTH = 10;
static const float MAX_RAY_DISTANCE = 100000.0f;
// Determines the contribution of reflected/refracted colours
// to the final colour of a surface.
static const float REFLECTED_REFRACTED_WEIGHT = 0.5f;
// Determines the contribution of local illumination
// to the final colour of a surface.
static const float LOCAL_ILLUMINATION_WEIGHT = (1 - REFLECTED_REFRACTED_WEIGHT);
// Used to prevent floating point error affecting shadow casting
static const float SHADOW_RAY_DISTANCE_THRESHOLD = 0.1f;

// Colour to give all test shapes
static const Colour TEST_SHAPE_COLOUR = Colour(1.0f, 1.0f, 0.5f);

class Raytracer
{

public:
    Raytracer(const Camera& camera);
    virtual ~Raytracer();

    /* Single and multisample raytracing. */
    bool raytrace(float x, float y, Colour& result);
    bool uniformMultisample(float minX, float minY, float maxX, float maxY,
    	unsigned int samplesPerDirection, Colour& result); /* produces (samplesPerDirection * samplesPerDirection) samples */
    bool randomMultisample(float minX, float minY, float maxX, float maxY,
        unsigned int samples, Colour& result);
    /* Methods which compute the contribution of different physical
     * phenoma to the final pixel colour. */
    Colour localIllumination(const Material* material, const Colour& objectColour, const HitRecord& record);
    Colour reflectionAndRefraction(const Vector3& rayDirection,
        const HitRecord& record, int depth);

    /* Set a new root shape in the shape hierarchy. */
    void setRootShape(Shape* newRoot, bool deletePrevious = true);
    Shape* getRootShape();
    /* Add another light ot the scene. */
    void addLight(const PointLight& light);
    /* Remove all lights from scene. */
    void removeAllLights();
    /* Retrieve reference to scene's camera. */
    Camera* getCamera();

    /* Enable/disable different effects. */
    void enableLocalIllumination(bool enabled);
    void enableReflectionAndRefraction(bool enabled);
    void enableShadows(bool enabled);

    /* Enable/disable test shapes from being rendered. */
    bool showingTestShapes() const;
    void showTestShapes(bool show);
    /* Sets a new test shape for the raytracer to render.
     * The Raytracer instance takes ownership of the shape
     * and deletes it when the raytracer is destroyed. */
    void setRootTestShape(Shape* newRootTest, bool deletePrevious = true);

    /* Accessors for statistics on trace. */
    unsigned int primaryRays() const;
    unsigned int reflectedRays() const;
    unsigned int refractedRays() const;
    unsigned int shadowRays() const;
    unsigned int totalRays() const;
    /* Used to reset ray counts to zero. */
    void resetRayCount();

private:
    /* Fire a ray into the scene and recursively trace the colour of
     * the hit pixel (stored in record.colour). */
    bool recursiveTrace(const Ray& ray, HitRecord& record, int depth);

    /* Used to compute reflection/refraction rays. */
    float computeSurfaceReflectivity(const Vector3& incoming,
        const Vector3& surfaceNormal, float originRefractiveIndex,
        float hitRefractiveIndex);
    bool computeRefractedRay(const Vector3 incidentDirection,
        const Vector3& pointOfIntersection, const Vector3& surfaceNormal,
        float refractiveIndex1, float refractiveIndex2,
        Ray& result);

    // Inforemation about the main scene to render
    Shape* rootShape;
    LightList lights;
    Camera camera;

    // Shapes which are rendered in debug mode. These ignore lighting,
    // shadows any other special effects. All shapes are rendered with
    // the same colour.
    Shape* rootTestShape;
    bool testShapesEnabled; // if true, test shapes will be drawn

    // Default material used if a shape does not have one specified
    Material defaultMaterial;

	bool localIllumEnabled;
	bool reflectRefractEnabled;
	bool shadowsEnabled;

    // Statistics on raytracer performance
    unsigned int numPrimaryRays;
    unsigned int numReflectedRays;
    unsigned int numRefractedRays;
    unsigned int numShadowRays;

};

}

#endif
