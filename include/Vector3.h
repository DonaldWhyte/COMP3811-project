#ifndef DW_RAYTRACER_VECTOR3_H
#define DW_RAYTRACER_VECTOR3_H

#include <iostream>

class Vector3
{

public:
    Vector3();
    Vector3(float x, float y, float z);
   
    /* Unary operators */
    const Vector3& operator+() const;
    Vector3 operator-() const;
    /* Subscript access */
    float operator[](int index) const; 
    float& operator[](int index);
    
    float length() const;
    float squaredLength() const;
    
    float minComponent() const;
    float maxComponent() const;
    float minMagnitudeComponent() const;    
    float maxMagnitudeComponent() const;
    
    Vector3 normalise() const;
    float dot(const Vector3& vec) const;
    Vector3 cross(const Vector3& vec) const;
    
    Vector3& operator=(const Vector3& vec);
    Vector3& operator+=(const Vector3& vec);
    Vector3& operator-=(const Vector3& vec);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);
   
    union
    {
        struct
        {
            float x, y, z;
        };
        float elems[3];
    };
   
};

bool operator==(const Vector3& v1, const Vector3& v2);
bool operator!=(const Vector3& v1, const Vector3& v2);

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator/(const Vector3& vec, float scalar);
Vector3 operator*(const Vector3& vec, float scalar);
Vector3 operator*(float scalar, const Vector3& vec);

namespace {
    std::ostream& operator<<(std::ostream& os, const Vector3& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
}
}

#endif
