#ifndef DW_RAYTRACER_RAYTRACER_H
#define DW_RAYTRACER_RAYTRACER_H

#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Ray.h"

static const int MAX_TRACE_DEPTH = 10;
static const float MAX_RAY_DISTANCE = 100000.0f;

class Raytracer
{

public:
    Raytracer(const Camera& camera);
    virtual ~Raytracer();

    bool raytrace(float x, float y, Colour& result);
    void addShape(Shape* shape);
    void addLight(const PointLight& light);
    Camera* getCamera();

private:
    void reset();
    bool recursiveTrace(const Ray& ray, HitRecord& record, int depth);

    ShapeList shapes;
    LightList lights;
    Camera camera;

};

#endif
