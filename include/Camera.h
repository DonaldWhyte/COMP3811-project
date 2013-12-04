#ifndef DW_RAYTRACER_CAMERA_H
#define DW_RAYTRACER_CAMERA_H

#include "Ray.h"

struct Rectangle
{
    float left, right;
    float top, bottom;

    Rectangle(float left, float right, float top, float bottom) :
        left(left), right(right), top(top), bottom(bottom)
    {
    }

};

class Camera
{

public:
    Camera(const Vector3& position, const Vector3& direction, const Vector3& up,
        float aperture, const Rectangle& viewingRectangle, float distance = DEFAULT_DISTANCE);

    Ray getRayToPixel(float pixelX, float pixelY, float xi1, float xi2);

private:
    static const float DEFAULT_DISTANCE = 1.0;

    /* Rebuild orthonormal basis vectors for camera based on
     * direction and up vector (third basis vector be computed
     * from those two vectors. */
    void updateBasisVectors(const Vector3& direction, const Vector3& up);

    // Position of camera
    Vector3 position;
    // Orthonormal basis (coordinate system) for camera
    // Computed from camera's direction and up vectors
    Vector3 u, v, w;
    // Distance from viewing rectangle
    float s; // typically just 1.0

    // Radius of lens (used for perspective)
    float lensRadius;

    // Coordinates of viewing rectangle
    Rectangle viewingRect;
    // Vectors across on bottom edge and upwards on left edge OF VIEWING RECENTANGLE
    Vector3 acrossVec;
    Vector3 upVec;
    // Point at bottom-left corner of VIEWING RECTANGLE
    Vector3 cornerPoint;

};

#endif
