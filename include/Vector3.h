#ifndef DW_RAYTRACER_VECTOR3_H
#define DW_RAYTRACER_VECTOR3_H

#include <iostream>
#include <cmath>
#include <algorithm>

namespace raytracer {

class Vector3
{

public:
    /* Vector components. */
    union
    {
        struct
        {
            float x, y, z;
        };
        float elems[3];
    };

    /* Non-member friend functions. */
    friend bool operator==(const Vector3& v1, const Vector3& v2);
    friend bool operator!=(const Vector3& v1, const Vector3& v2);

    friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator-(const Vector3& v1, const Vector3& v2);
    friend Vector3 operator/(const Vector3& vec, float scalar);
    friend Vector3 operator*(const Vector3& vec, float scalar);
    friend Vector3 operator*(float scalar, const Vector3& vec);

    inline Vector3() : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    inline Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    /* Unary operators. */
    inline const Vector3& operator+() const
    {
        return *this;
    }

    inline Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    /* Subscript access. */
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
        return sqrt(x * x + y * y + z * z);
    }

    inline float squaredLength() const
    {
        return x * x + y * y + z * z;
    }

    inline float minComponent() const
    {
        return std::min(x, std::min(y, z));
    }

    inline float maxComponent() const
    {
        return std::max(x, std::max(y, z));
    }

    inline float minMagnitudeComponent() const
    {
        float temp = fabs(x);
        if (fabs(y) < temp) temp = fabs(y);
        if (fabs(z) < temp) temp = fabs(z);
        return temp;
    }

    inline float maxMagnitudeComponent() const
    {
        float temp = fabs(x);
        if (fabs(y) > temp) temp = fabs(y);
        if (fabs(z) > temp) temp = fabs(z);
        return temp;
    }

    inline Vector3& operator=(const Vector3& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    inline Vector3& operator+=(const Vector3& vec)
    {
        *this = *this + vec;
        return *this;
    }

    inline Vector3& operator-=(const Vector3& vec)
    {
        *this = *this - vec;
        return *this;
    }

    inline Vector3& operator*=(float scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    inline Vector3 normalise() const
    {
        return (*this) / length();
    }

    inline float dot(const Vector3& vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    inline Vector3 cross(const Vector3& vec) const
    {
        return Vector3(
            y * vec.z - z * vec.y,
            z * vec.x - x * vec.z,
            x * vec.y - y * vec.x
        );
    }

};

/* Non-member operators. */
inline bool operator==(const Vector3& v1, const Vector3& v2)
{
    return (!(
        v1.x != v2.x ||
        v1.y != v2.y ||
        v1.z != v2.z
    ));
}

inline bool operator!=(const Vector3& v1, const Vector3& v2)
{
    return !(v1 == v2);
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3 operator/(const Vector3& vec, float scalar)
{
    return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

inline Vector3 operator*(const Vector3& vec, float scalar)
{
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

inline Vector3 operator*(float scalar, const Vector3& vec)
{
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

namespace {
    std::ostream& operator<<(std::ostream& os, const Vector3& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }
}

}

#endif
