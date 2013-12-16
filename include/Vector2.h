#ifndef DW_RAYTRACER_VECTOR2_H
#define DW_RAYTRACER_VECTOR2_H

#include <iostream>
#include <cmath>
#include <algorithm>

namespace raytracer {

class Vector2
{

public:
    /* Vector components. */
    union
    {
        struct
        {
            float x, y;
        };
        float elems[2];
    };

    /* Non-member friend functions. */
    friend bool operator==(const Vector2& v1, const Vector2& v2);
    friend bool operator!=(const Vector2& v1, const Vector2& v2);

    friend Vector2 operator+(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator-(const Vector2& v1, const Vector2& v2);
    friend Vector2 operator/(const Vector2& vec, float scalar);
    friend Vector2 operator*(const Vector2& vec, float scalar);
    friend Vector2 operator*(float scalar, const Vector2& vec);

    inline Vector2() : x(0.0f), y(0.0f)
    {
    }

    inline Vector2(float x, float y) : x(x), y(y)
    {
    }

    inline const Vector2& operator+() const
    {
        return *this;
    }

    inline Vector2 operator-() const
    {
        return Vector2(-x, -y);
    }

    inline float operator[](int index) const
    {
        return elems[index];
    }

    inline float& operator[](int index)
    {
        return elems[index];
    }

    inline float length() const
    {
        return sqrt(x * x + y * y);
    }

    inline float squaredLength() const
    {
        return x * x + y * y;
    }

    inline float minComponent() const
    {
        return std::min(x, y);
    }

    inline float maxComponent() const
    {
        return std::max(x, y);
    }

    inline float minMagnitudeComponent() const
    {
        float temp = fabs(x);
        if (fabs(y) < temp) temp = fabs(y);
        return temp;
    }

    inline float maxMagnitudeComponent() const
    {
        float temp = fabs(x);
        if (fabs(y) > temp) temp = fabs(y);
        return temp;
    }

    inline Vector2& operator=(const Vector2& vec)
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    inline Vector2& operator+=(const Vector2& vec)
    {
        *this = *this + vec;
        return *this;
    }

    inline Vector2& operator-=(const Vector2& vec)
    {
        *this = *this - vec;
        return *this;
    }

    inline Vector2& operator*=(float scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    inline Vector2 normalise() const
    {
        return (*this) / length();
    }

    inline float dot(const Vector2& vec) const
    {
        return x * vec.x + y * vec.y;
    }

};

/* Non-member functions. */
inline bool operator==(const Vector2& v1, const Vector2& v2)
{
    return (!(
        v1.x != v2.x ||
        v1.y != v2.y
    ));
}

inline bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return !(v1 == v2);
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2 operator/(const Vector2& vec, float scalar)
{
    return Vector2(vec.x / scalar, vec.y / scalar);
}

inline Vector2 operator*(const Vector2& vec, float scalar)
{
    return Vector2(vec.x * scalar, vec.y * scalar);
}

inline Vector2 operator*(float scalar, const Vector2& vec)
{
    return Vector2(vec.x * scalar, vec.y * scalar);
}

namespace {
    std::ostream& operator<<(std::ostream& os, const Vector2& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }
}

}

#endif
