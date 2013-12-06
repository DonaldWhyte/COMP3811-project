#include "Vector2.h"
#include <cmath>
#include <algorithm>

Vector2::Vector2() : x(0.0f), y(0.0f)
{
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

const Vector2& Vector2::operator+() const
{
    return *this;
}

Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

float Vector2::operator[](int index) const
{
    return elems[index];
}

float& Vector2::operator[](int index)
{
    return elems[index];
}

float Vector2::length() const
{
    return sqrt(x * x + y * y);
}

float Vector2::squaredLength() const
{
    return x * x + y * y;
}

float Vector2::minComponent() const
{
    return std::min(x, y);
}
float Vector2::maxComponent() const
{
    return std::max(x, y);
}

float Vector2::minMagnitudeComponent() const
{
    float temp = fabs(x);
    if (fabs(y) < temp) temp = fabs(y);
    return temp;
}

float Vector2::maxMagnitudeComponent() const
{
    float temp = fabs(x);
    if (fabs(y) > temp) temp = fabs(y);
    return temp;
}

Vector2& Vector2::operator=(const Vector2& vec)
{
    x = vec.x;
    y = vec.y;
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
    *this = *this + vec;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
    *this = *this - vec;
    return *this;
}

Vector2& Vector2::operator*=(float scalar)
{
    *this = *this * scalar;
    return *this;
}

Vector2 Vector2::normalise() const
{
    return (*this) / length();
}

float Vector2::dot(const Vector2& vec) const
{
    return x * vec.x + y * vec.y;
}

bool operator==(const Vector2& v1, const Vector2& v2)
{
    return (!(
        v1.x != v2.x ||
        v1.y != v2.y
    ));
}

bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return !(v1 == v2);
}

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 operator/(const Vector2& vec, float scalar)
{
    return Vector2(vec.x / scalar, vec.y / scalar);
}

Vector2 operator*(const Vector2& vec, float scalar)
{
    return Vector2(vec.x * scalar, vec.y * scalar);
}

Vector2 operator*(float scalar, const Vector2& vec)
{
    return Vector2(vec.x * scalar, vec.y * scalar);
}
