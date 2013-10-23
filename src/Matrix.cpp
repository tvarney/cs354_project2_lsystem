
#include "Matrix.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include "common.hpp"

using namespace cs354;

Matrix::Matrix() { }
Matrix::Matrix(float mData[16]) {
    data[0] = mData[0];   data[1] = mData[1];   data[2] = mData[2];
    data[3] = mData[3];   data[4] = mData[4];   data[5] = mData[5];
    data[6] = mData[6];   data[7] = mData[7];   data[8] = mData[8];
    data[9] = mData[9];   data[10] = mData[10]; data[11] = mData[11];
    data[12] = mData[12]; data[13] = mData[13]; data[14] = mData[14];
    data[15] = mData[15];
}
Matrix::Matrix(const Matrix &source) {
    data[0] = source.data[0];   data[1] = source.data[1];
    data[2] = source.data[2];   data[3] = source.data[3];
    data[4] = source.data[4];   data[5] = source.data[5];
    data[6] = source.data[6];   data[7] = source.data[7];
    data[8] = source.data[8];   data[9] = source.data[9];
    data[10] = source.data[10]; data[11] = source.data[11];
    data[12] = source.data[12]; data[13] = source.data[13];
    data[14] = source.data[14]; data[15] = source.data[15];
}

Matrix::~Matrix() { }

void Matrix::use() {
    glLoadMatrixf(data);
}

Matrix & Matrix::operator=(const Matrix &rhs) {
    std::memcpy(data, rhs.data, sizeof(float) * 16);
    return (*this);
}

Matrix & Matrix::operator*=(const Matrix &rhs) {
    return ((*this) = (*this) * rhs);
}

#define POS(r,c) ((c) * 4 + (r))
#define MUL(lhs, rhs, r,c)                              \
    (lhs).data[POS(r,0)] * (rhs).data[POS(0,c)] +       \
    (lhs).data[POS(r,1)] * (rhs).data[POS(1,c)] +       \
    (lhs).data[POS(r,2)] * (rhs).data[POS(2,c)] +       \
    (lhs).data[POS(r,3)] * (rhs).data[POS(3,c)]

Matrix Matrix::operator*(const Matrix &rhs) {
    Matrix retval;
    retval.data[POS(0,0)] = MUL(*this, rhs, 0, 0);
    retval.data[POS(1,0)] = MUL(*this, rhs, 1, 0);
    retval.data[POS(2,0)] = MUL(*this, rhs, 2, 0);
    retval.data[POS(3,0)] = MUL(*this, rhs, 3, 0);
    retval.data[POS(0,1)] = MUL(*this, rhs, 0, 1);
    retval.data[POS(1,1)] = MUL(*this, rhs, 1, 1);
    retval.data[POS(2,1)] = MUL(*this, rhs, 2, 1);
    retval.data[POS(3,1)] = MUL(*this, rhs, 3, 1);
    retval.data[POS(0,2)] = MUL(*this, rhs, 0, 2);
    retval.data[POS(1,2)] = MUL(*this, rhs, 1, 2);
    retval.data[POS(2,2)] = MUL(*this, rhs, 2, 2);
    retval.data[POS(3,2)] = MUL(*this, rhs, 3, 2);
    retval.data[POS(0,3)] = MUL(*this, rhs, 0, 3);
    retval.data[POS(1,3)] = MUL(*this, rhs, 1, 3);
    retval.data[POS(2,3)] = MUL(*this, rhs, 2, 3);
    retval.data[POS(3,3)] = MUL(*this, rhs, 3, 3);
    return retval;
}

Matrix & Matrix::translate(float x, float y, float z) {
    (*this) *= Matrix::Matrix3D(1.0, 0.0, 0.0, x,
                                0.0, 1.0, 0.0, y,
                                0.0, 0.0, 1.0, z,
                                0.0, 0.0, 0.0, 1.0);
    return (*this);
}
Matrix & Matrix::translate(Vector3f vec) {
    return translate(vec.x, vec.y, vec.z);
}

/* Should be faster, less calculation */
Matrix & Matrix::rotate(float angle, Axis axis) {
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    switch(axis) {
    case AXIS_X:
        (*this) *= Matrix::Matrix3D(1, 0,    0,   0,
                                    0, cos, -sin, 0,
                                    0, sin,  cos, 0,
                                    0, 0,    0,   1);
        break;
    case AXIS_Y:
        (*this) *= Matrix::Matrix3D( cos, 0, sin, 0,
                                     0,   1, 0,   0,
                                    -sin, 0, cos, 0,
                                     0,   0, 0,   1);
        break;
    case AXIS_Z:
        (*this) *= Matrix::Matrix3D(cos, -sin, 0, 0,
                                    sin,  cos, 0, 0,
                                    0,    0,   1, 0,
                                    0,    0,   0, 1);
        break;
    }
    return (*this);
}
/* More general */
Matrix & Matrix::rotate(float angle, Vector3f line) {
    return rotate(angle, line.x, line.y, line.z);
}
Matrix & Matrix::rotate(float angle, float x, float y, float z) {
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    float cosm1 = cos - 1.0;
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    float L = x2 + y2 + z2;
    float sqrtL = std::sqrt(L);
    float invL = 1.0/L;
    
    Matrix mat =
        Matrix3D((x2+(y2+z2)*cos)*invL,
                 (x*y*cosm1-z*sqrtL*sin)*invL,
                 (x*z*cosm1+y*sqrtL*sin)*invL,
                 ((x*(y2+z2)-x*(y2+z2))*cosm1+(y*z-z*y)*sqrtL*sin)*invL,
                 (x*y*cosm1+z*sqrtL*sin)*invL,
                 (y2+(x2+z2)*cos)*invL,
                 (y*z*cosm1-x*sqrtL*sin)*invL,
                 ((y*(x2+z2)-y*(x2+z2))*cosm1+(z*x-x*z)*sqrtL*sin)*invL,
                 (x*z*cosm1*y*sqrtL*sin)*invL,
                 (y*z*cosm1+x*sqrtL*sin)*invL,
                 (z2+(x2+y2)*cos)*invL,
                 ((z*(x2+y2)-z*(x2+y2))*cosm1+(x*y-y*x)*sqrtL*sin)*invL,
                 0,0,0,1);
    
    (*this) *= mat;
    return (*this);
}

Matrix & Matrix::scale(float x, float y, float z) {
    (*this) *= Matrix3D(x, 0, 0, 0,
                        0, y, 0, 0,
                        0, 0, z, 0,
                        0, 0, 0, 1);
    return *this;
}

static const char _printf_fmt[] =
    "%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n";
void Matrix::print() {
    std::printf(_printf_fmt,
                data[POS(0,0)], data[POS(0,1)], data[POS(0,2)], data[POS(0,3)],
                data[POS(1,0)], data[POS(1,1)], data[POS(1,2)], data[POS(1,3)],
                data[POS(2,0)], data[POS(2,1)], data[POS(2,2)], data[POS(2,3)],
                data[POS(3,0)], data[POS(3,1)], data[POS(3,2)], data[POS(3,3)]
                );
}

/**
 * | 1.0 0.0 0.0 0.0 |
 * | 0.0 1.0 0.0 0.0 |
 * | 0.0 0.0 1.0 0.0 |
 * | 0.0 0.0 0.0 1.0 |
 */
static float _identity[16] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};
Matrix Matrix::Identity() {
    return Matrix(_identity);
}

Matrix Matrix::Matrix2D(float m00, float m01, float m02,
                        float m10, float m11, float m12,
                        float m20, float m21, float m22)
{
    Matrix rval;
    rval.data[0] = m00;  rval.data[1] = m10;  rval.data[2] = 0.0;
    rval.data[3] = m20;  rval.data[4] = m01;  rval.data[5] = m11;
    rval.data[6] = 0.0;  rval.data[7] = m21;  rval.data[8] = 0.0;
    rval.data[9] = 0.0;  rval.data[10] = 1.0; rval.data[11] = 0.0;
    rval.data[12] = m02; rval.data[13] = m12; rval.data[14] = 0.0;
    rval.data[15] = m22;
    return rval;
}
Matrix Matrix::Matrix3D(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33)
{
    Matrix rval;
    rval.data[0] = m00;  rval.data[1] = m10;  rval.data[2] = m20;
    rval.data[3] = m30;  rval.data[4] = m01;  rval.data[5] = m11;
    rval.data[6] = m21;  rval.data[7] = m31;  rval.data[8] = m02;
    rval.data[9] = m12;  rval.data[10] = m22; rval.data[11] = m32;
    rval.data[12] = m03; rval.data[13] = m13; rval.data[14] = m23;
    rval.data[15] = m33;
    return rval;
}

Matrix Matrix::LookAt(Vector3f eye) {
    return Matrix::LookAt(eye, Vector3f(0,0,0), Vector3f(0,1,0));
}

Matrix Matrix::LookAt(Vector3f eye, Vector3f up) {
    return Matrix::LookAt(eye, Vector3f(0,0,0), up);
}

Matrix Matrix::LookAt(Vector3f eye, Vector3f center, Vector3f up) {
    Vector3f forward = (center - eye).normalize();
    Vector3f side = (forward * up).normalize();
    up = (side * forward);
    
    Matrix mat = Matrix::Matrix3D(side.x,     side.y,     side.z,     0,
                                  up.x,       up.y,       up.z,       0,
                                  -forward.x, -forward.y, -forward.z, 0,
                                  0,          0,          0,          1);
    mat.translate(-eye);
    return mat;
}
