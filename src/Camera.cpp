#include "Camera.h"

Camera::Camera(const Vector3& position, const Vector3& direction, const Vector3& up,
    float aperture, const Rectangle& viewingRectangle, float distance) :
    position(position), s(distance), lensRadius(aperture / 2.0f), viewingRect(viewingRectangle)
{
    updateBasisVectors(direction, up);
    acrossVec = (viewingRect.left - viewingRect.right) * u;
    upVec = (viewingRect.bottom - viewingRect.top) * v;
    cornerPoint = position + (viewingRect.left * u) + (viewingRect.bottom * v) - (s * w);
}

Ray Camera::getRayToPixel(float pixelX, float pixelY, float xi1, float xi2)
{
    // Compute ray origin based on camera position
    /*Vector3 origin = position
        + 2.0f * (xi1 - 0.5f) * lensRadius * u
        + 2.0f * (xi2 - 0.5f) * lensRadius * v;*/
    Vector3 origin = position;

    // Compute vector towards target from camera to get ray's direction
    Vector3 target = cornerPoint + acrossVec * pixelX + upVec * pixelY; // point on screen
    Vector3 direction = (target - origin).normalise(); // (s - e)

    return Ray(origin, direction);
}

void Camera::updateBasisVectors(const Vector3& direction, const Vector3& up)
{
    // Construct orthonormal basis vectors for view coordinate system
    w = (-direction).normalise();
    u = (-direction).cross(up).normalise();
    v = w.cross(u);
}
