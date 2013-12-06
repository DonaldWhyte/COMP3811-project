#ifndef DW_RAYTRACER_VECTOR2_H
#define DW_RAYTRACER_VECTOR2_H

#include <iostream>

class Vector2
{

public:
    Vector2();
    Vector2(float x, float y);

    /* Unary operators */
    const Vector2& operator+() const;
    Vector2 operator-() const;
    /* Subscript access */
    float operator[](int index) const;
    float& operator[](int index);

    float length() const;
    float squaredLength() const;

    float minComponent() const;
    float maxComponent() const;
    float minMagnitudeComponent() const;
    float maxMagnitudeComponent() const;

    Vector2 normalise() const;
    float dot(const Vector2& vec) const;

    Vector2& operator=(const Vector2& vec);
    Vector2& operator+=(const Vector2& vec);
    Vector2& operator-=(const Vector2& vec);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    union
    {
        struct
        {
            float x, y;
        };
        float elems[2];
    };

};

bool operator==(const Vector2& v1, const Vector2& v2);
bool operator!=(const Vector2& v1, const Vector2& v2);

Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator/(const Vector2& vec, float scalar);
Vector2 operator*(const Vector2& vec, float scalar);
Vector2 operator*(float scalar, const Vector2& vec);

namespace {
    std::ostream& operator<<(std::ostream& os, const Vector2& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
}
}

#endif
