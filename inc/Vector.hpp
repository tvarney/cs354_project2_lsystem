
#ifndef CS354_GENERIC_VECTOR_HPP
#define CS354_GENERIC_VECTOR_HPP

#include <cmath>

namespace cs354 {
    /* Vector<T>
     * A 3 dimensional vector.
     * Should only be used with primitives or very simple types you don't mind
     * copying around.
     * I really don't like templates, but eh.
     */
    template <typename T> struct Vector {
        /* Constructors/Destructor */
        Vector(const Vector<T> &vec) :
            x(vec.x), y(vec.y), z(vec.z)
        { }
        Vector(T x, T y, T z) :
            x(x), y(y), z(z)
        { }
        Vector(T args[3]) :
            x(args[0]), y(args[1]), z(args[2])
        { }
        ~Vector() { }
        
        /* Operators */
        /* Operator: +v1 */
        Vector<T> operator+() const {
            return Vector<T>(*this);
        }
        /* Operator: -v1 */
        Vector<T> operator-() const {
            return Vector<T>(-x, -y, -z);
        }
        
        /* Operator: v1 + v2 */
        Vector<T> operator+(const Vector<T> &rhs) const {
            return Vector<T>(x + rhs.x, y + rhs.y, z + rhs.z);
        }
        /* Operator: v1 += v2 */
        Vector<T> & operator+=(const Vector<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return (*this);
        }
        /* Operator: v1 - v2 */
        Vector<T> operator-(const Vector<T> &rhs) const {
            return Vector<T>(x - rhs.x, y - rhs.y, z - rhs.z);
        }
        /* Operator: v1 -= v2 */
        Vector<T> & operator-=(const Vector<T> &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return (*this);
        }
        /* Operator: v1 * s */
        Vector<T> operator*(T scalar) const {
            return Vector<T>(x * scalar, y * scalar, z * scalar);
        }
        /* Operator: v1 *= s */
        Vector<T> & operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return (*this);
        }
        
        /* Operator: v1 * v2 */
        Vector<T> operator*(const Vector<T> &rhs) const {
            return Vector<T>(y * rhs.z - z * rhs.y,
                             z * rhs.x - x * rhs.z,
                             x * rhs.y - y * rhs.x);
        }
        /* Operator: v1 *= v2 */
        Vector<T> & operator*=(const Vector<T> &rhs) const {
            return (*this = *this * rhs);
        }
        
        /* Operator: v1 = v2 */
        Vector<T> & operator=(const Vector<T> &rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return (*this);
        }
        /* Operator: v1[i] */
        T operator[](size_t index) {
            switch(index) {
            case 1:
                return x;
            case 2:
                return y;
            case 3:
                return z;
            default:
                return 0;
            }
        }
        
        /* Normalize the vector */
        Vector<T> & normalize() {
            T len = std::sqrt(x * x + y * y + z * z);
            x /= len;
            y /= len;
            z /= len;
            return (*this);
        };
        
        T x, y, z;
    };
    
    typedef Vector<float> Vector3f;
    typedef Vector<double> Vector3d;
}

#endif
