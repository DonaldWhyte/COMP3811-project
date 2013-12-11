#include "Camera.h"

Camera::Camera(const Vector3& position, const Vector3& direction, const Vector3& up,
    const Rectangle& viewingRectangle, float distance, bool orthographic) :
    position(position), distance(distance), viewingRect(viewingRectangle),
    orthographic(orthographic)
{
    updateBasisVectors(direction, up);
    acrossVec = (viewingRect.right - viewingRect.left) * u;
    upVec = (viewingRect.top - viewingRect.bottom) * v;
    cornerPoint = position + (viewingRect.left * u) + (viewingRect.bottom * v) - (distance * w);
}

Ray Camera::getRayToPixel(float pixelX, float pixelY)
{
    // Compute position of point on screen to render
    Vector3 target = cornerPoint + acrossVec * pixelX + upVec * pixelY;
    if (orthographic) // orthographic projection
    {
        return Ray(target, acrossVec.cross(upVec).normalise());
    }
    else // perspective projection
    {
        // Ray origin is simply camera position
        Vector3 origin = position;
        // Compute vector towards target from camera to get ray's direction
        Vector3 direction = (target - origin).normalise(); // (s - e)
        return Ray(origin, direction);
    }
}

bool Camera::isOrthographic() const
{
    return orthographic;
}

void Camera::setOrthographic(bool useOrthographicProjection)
{
    orthographic = useOrthographicProjection;
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
    u = up.cross(w).normalise();
    v = w.cross(u);
}
