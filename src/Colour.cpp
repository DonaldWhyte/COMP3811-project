#include "Colour.h"

using namespace raytracer;

Colour::Colour(float r, float g, float b) : r(r), g(g), b(b)
{
}

void Colour::clamp()
{
    if (r < 0.0f) r = 0.0f;
    if (g < 0.0f) g = 0.0f;
    if (b < 0.0f) b = 0.0f;
    if (r > 1.0f) r = 1.0f;
    if (g > 1.0f) g = 1.0f;
    if (b > 1.0f) b = 1.0f;
}

Colour& Colour::operator=(const Colour& other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

Colour& Colour::operator+=(const Colour& other)
{
    *this = *this + other;
    return *this;
}

Colour& Colour::operator-=(const Colour& other)
{
    *this = *this + (-other);
    return *this;
}

Colour& Colour::operator*=(const Colour& other)
{
    *this = *this * other;
    return *this;
}

Colour& Colour::operator/=(const Colour& other)
{
    *this = *this / other;
    return *this;
}

Colour Colour::operator+() const
{
    return *this;
}

Colour Colour::operator-() const
{
    return Colour(-r, -g, -b);
}

Colour raytracer::operator*(const Colour& a, float k)
{
    return Colour(a.r * k, a.g * k, a.b * k);
}

Colour raytracer::operator*(float k, const Colour& a)
{
    return Colour(a.r * k, a.g * k, a.b * k);
}

Colour raytracer::operator/(const Colour& a, float k)
{
    return Colour(a.r / k, a.g / k, a.b / k);
}

Colour raytracer::operator*(const Colour& a, const Colour& b)
{
    return Colour(a.r * b.r, a.g * b.g, a.b * b.b);
}

Colour raytracer::operator/(const Colour& a, const Colour& b)
{
    return Colour(a.r / b.r, a.g / b.g, a.b / b.b);
}

Colour raytracer::operator+(const Colour& a, const Colour& b)
{
    return Colour(a.r + b.r, a.g + b.g, a.b + b.b);
}
