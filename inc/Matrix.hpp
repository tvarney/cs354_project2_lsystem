
#ifndef CS354_GENERIC_MATRIX_HPP
#define CS354_GENERIC_MATRIX_HPP

#include <cstring>

#include "Vector.hpp"

namespace cs354 {
    enum Axis {
        AXIS_X,
        AXIS_Y,
        AXIS_Z
    };
    
    struct Matrix {
        float data[16]; /*< Column Major order */
        
        /* Constructors/Destructors */
        Matrix();
        Matrix(float mData[16]);
        Matrix(const Matrix &source);
        ~Matrix();
        
        /* OpenGL binding */
        void use();
        
        /* Operators */
        Matrix & operator=(const Matrix &rhs);
        Matrix & operator*=(const Matrix &rhs);
        Matrix operator*(const Matrix &rhs);
        
        /* Mutation Methods */
        Matrix & translate(float x, float y, float z);
        Matrix & translate(Vector3f vec);
        Matrix & rotate(float angle, Axis axis);
        Matrix & rotate(float angle, float x, float y, float z);
        Matrix & rotate(float angle, Vector3f line);
        Matrix & scale(float x, float y, float z);
        /* Not implemented */
        /*
          Matrix & transpose();
          Matrix & inverse();
          
          Matrix operator+();
          Matrix operator-();
        
          Matrix & operator+=(const Matrix &rhs);
          Matrix operator+(const Matrix &rhs);
          Matrix & operator-=(const Matrix &rhs);
          Matrix operator-(const Matrix &rhs);
          Matrix & operator*=(const Vector3f rhs);
          Matrix operator*(const Vector3f rhs);
         */
        
        /* Output */
        void print();
        
        /* Static interface */
        /* Return the identity matrix */
        static Matrix Identity();
        /* Return a 3D version of the given row-major 2D matrix */
        static Matrix Matrix2D(float m00, float m01, float m02,
                               float m10, float m11, float m12,
                               float m20, float m21, float m22);
        /* Return a 3D version of the given row-major 3D matrix */
        static Matrix Matrix3D(float m00, float m01, float m02, float m03,
                               float m10, float m11, float m12, float m13,
                               float m20, float m21, float m22, float m23,
                               float m30, float m31, float m32, float m33);
        
        static Matrix LookAt(Vector3f eye);
        static Matrix LookAt(Vector3f eye, Vector3f up);
        static Matrix LookAt(Vector3f eye, Vector3f center, Vector3f up);
    };
}

#endif
