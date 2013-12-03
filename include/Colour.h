#ifndef DW_RAYTRACE_COLOUR_H
#define DW_RAYTRACE_COLOUR_H

#include <iostream>

class Colour
{

public:

   Colour(float r = 0.0f, float g = 0.0f, float b = 0.0f);

   void clamp();

   Colour& operator=(const Colour& other);
   Colour& operator+=(const Colour& other);
   Colour& operator-=(const Colour& other);
   Colour& operator*=(const Colour& other);
   Colour& operator/=(const Colour& other);
   Colour& operator+=(float scalar);

   Colour operator+() const;
   Colour operator-() const;

   float r, g, b;

};

Colour operator*(const Colour& a, float k);
Colour operator*(float k, const Colour& a);
Colour operator/(const Colour& a, float k);
Colour operator*(const Colour& a, const Colour& b);
Colour operator/(const Colour& a, const Colour& b);
Colour operator+(const Colour& a, const Colour& b);

namespace
{
    std::ostream& operator<<(std::ostream& out, const Colour& colour)
    {
        out << "(" << colour.r << ", " << colour.g << ", " << colour.b << ")";
        return out;
    }
}

#endif
