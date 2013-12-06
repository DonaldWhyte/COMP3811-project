#ifndef DW_RAYTRACER_RAYTRACER_H
#define DW_RAYTRACER_RAYTRACER_H

#include "Shape.h"
#include "Camera.h"
#include "Ray.h"

class Raytracer
{

public:
    Raytracer(const Camera& camera, float tMax);
    virtual ~Raytracer();

    bool raytrace(float x, float y, Colour& result);
    void addShape(Shape* shape);
    Camera* getCamera();

private:
    void reset();
    bool recursiveTrace(const Ray& ray, HitRecord& record, int depth);

    ShapeList shapes;
    Camera camera;

    float initialTMax;
    float tMax;

};

#endif
