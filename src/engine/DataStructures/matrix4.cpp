#include "../include/engine/DataStructures/matrix4.h"
#include <cmath>

Matrix4::Matrix4() {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
}

Matrix4 Matrix4::identity() {
    Matrix4 mat;
    mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
    return mat;
}

Matrix4 Matrix4::translation(const Vector3& position) {
    Matrix4 mat = Matrix4::identity();
    mat.m[12] = position.x;
    mat.m[13] = position.y;
    mat.m[14] = position.z;
    return mat;
}

Matrix4 Matrix4::rotationX(float angle) {
    Matrix4 mat = Matrix4::identity();
    float c = cos(angle);
    float s = sin(angle);
    mat.m[5] = c;
    mat.m[6] = s;
    mat.m[9] = -s;
    mat.m[10] = c;
    return mat;
}

Matrix4 Matrix4::rotationY(float angle) {
    Matrix4 mat = Matrix4::identity();
    float c = cos(angle);
    float s = sin(angle);
    mat.m[0] = c;
    mat.m[2] = -s;
    mat.m[8] = s;
    mat.m[10] = c;
    return mat;
}

Matrix4 Matrix4::rotationZ(float angle) {
    Matrix4 mat = Matrix4::identity();
    float c = cos(angle);
    float s = sin(angle);
    mat.m[0] = c;
    mat.m[1] = s;
    mat.m[4] = -s;
    mat.m[5] = c;
    return mat;
}

Matrix4 Matrix4::scaling(const Vector3& scale) {
    Matrix4 mat = Matrix4::identity();
    mat.m[0] = scale.x;
    mat.m[5] = scale.y;
    mat.m[10] = scale.z;
    return mat;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i + j * 4] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i + j * 4] += m[i + k * 4] * other.m[k + j * 4];
            }
        }
    }
    return result;
}
