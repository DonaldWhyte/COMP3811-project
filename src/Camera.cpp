#include "Camera.h"

Camera::Camera(const Vector3& position, const Vector3& direction, const Vector3& up,
    const Rectangle& viewingRectangle, float distance) :
    position(position), distance(distance), viewingRect(viewingRectangle)
{
    updateBasisVectors(direction, up);
    acrossVec = (viewingRect.right - viewingRect.left) * u;
    upVec = (viewingRect.top - viewingRect.bottom) * v;
    cornerPoint = position + (viewingRect.left * u) + (viewingRect.bottom * v) - (distance * w);
}

Ray Camera::getRayToPixel(float pixelX, float pixelY, float xi1, float xi2)
{
    // Ray origin is simply camera position
    Vector3 origin = position;
    // Compute vector towards target from camera to get ray's direction
    Vector3 target = cornerPoint + acrossVec * pixelX + upVec * pixelY; // point on screen
    Vector3 direction = (target - origin).normalise(); // (s - e)

    return Ray(origin, direction);
}

const Vector3& Camera::getBasisX() const
{
    return u;
}

const Vector3& Camera::getBasisY() const
{
    return v;
}

const Vector3& Camera::getBasisZ() const
{
    return w;
}

void Camera::updateBasisVectors(const Vector3& direction, const Vector3& up)
{
    // Construct orthonormal basis vectors for view coordinate system
    w = (-direction).normalise();
    u = (-direction).cross(up).normalise();
    v = w.cross(u);
}
