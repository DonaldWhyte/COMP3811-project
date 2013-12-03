#include "Vector3.h"
#include <cmath>
#include <algorithm>

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

const Vector3& Vector3::operator+() const
{
    return *this;
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

float Vector3::operator[](int index) const
{
    return elems[index];
}

float& Vector3::operator[](int index)
{
    return elems[index];
}

float Vector3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vector3::squaredLength() const
{
    return x * x + y * y + z * z;
}

float Vector3::minComponent() const
{
    return std::min(x, std::min(y, z));
}

float Vector3::maxComponent() const
{
    return std::max(x, std::max(y, z));
}

float Vector3::minMagnitudeComponent() const
{
    float temp = fabs(x);
    if (fabs(y) < temp) temp = fabs(y);
    if (fabs(z) < temp) temp = fabs(z);
    return temp;
}
    
float Vector3::maxMagnitudeComponent() const
{
    float temp = fabs(x);
    if (fabs(y) > temp) temp = fabs(y);
    if (fabs(z) > temp) temp = fabs(z);
    return temp;
}

Vector3& Vector3::operator=(const Vector3& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& vec)
{
    *this = *this + vec;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& vec)
{
    *this = *this - vec;
    return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
    *this = *this * scalar;
    return *this;
}

Vector3 Vector3::normalise() const
{
    return (*this) / length();
}

float Vector3::dot(const Vector3& vec) const
{
    return x * vec.x + y * vec.y + z * vec.z;
}

Vector3 Vector3::cross(const Vector3& vec) const
{
    return Vector3(
        y * vec.z - z * vec.y,
        z * vec.x - x * vec.z,
        x - vec.y - y * vec.y
    );
}

bool operator==(const Vector3& v1, const Vector3& v2)
{
    return (!(
        v1.x != v2.x ||
        v1.y != v2.y ||
        v1.z != v2.z
    ));
}

bool operator!=(const Vector3& v1, const Vector3& v2)
{
    return !(v1 == v2);
}

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator/(const Vector3& vec, float scalar)
{
    return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

Vector3 operator*(const Vector3& vec, float scalar)
{
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

Vector3 operator*(float scalar, const Vector3& vec)
{
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}
