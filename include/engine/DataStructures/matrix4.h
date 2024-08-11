#ifndef MATRIX4_H
#define MATRIX4_H

#include "vector3.h"

class Matrix4 {
public:
    float m[16];

    Matrix4();
    static Matrix4 identity();
    static Matrix4 translation(const Vector3& position);
    static Matrix4 rotationX(float angle);
    static Matrix4 rotationY(float angle);
    static Matrix4 rotationZ(float angle);
    static Matrix4 scaling(const Vector3& scale);

    Matrix4 operator*(const Matrix4& other) const;
};

#endif // MATRIX4_H
