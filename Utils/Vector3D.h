#pragma once
#include "Point3D.h"

namespace Utils
{

template <typename T>
class Vector3D
{
private:
  T xp, yp, zp;

public:
  inline Vector3D(T x, T y, T z) : xp(x), yp(y), zp(z)
  {
  }

  inline Vector3D(T x1, T y1, T z1, T x2, T y2, T z2)
    : xp(x2 - x1), yp(y2 - y1), zp(z2 - z1)
  {
  }

  inline Vector3D(const Point3D<T>& a, const Point3D<T>& b)
    : xp(b.x() - a.x()), yp(b.y() - a.y()), zp(b.z() - a.z())
  {
  }

  virtual ~Vector3D()
  {}

  /// Returns X component.
  inline T x() const
  {
    return xp;
  }

  /// Returns Y component.
  inline T y() const
  {
    return yp;
  }

  /// Returns Z component.
  inline T z() const
  {
    return zp;
  }

  /// Sets X component.
  inline void setX(T value)
  {
    xp = value;
  }

  /// Sets Y component.
  inline void setY(T value)
  {
    yp = value;
  }

  /// Sets Z component.
  inline void setZ(T value)
  {
    zp = value;
  }

  /// Sets components.
  inline void set(T x, T y, T z)
  {
    xp = x;
    yp = y;
    zp = z;
  }

  /// Sets components. (overload)
  inline void set(const Vector3D<T>& v)
  {
    xp = v.xp;
    yp = v.yp;
    zp = v.zp;
  }

  /// Returns length (norm) squared.
  inline T lengthSquared() const
  {
    return xp * xp + yp * yp + zp * zp;
  }

  /// Returns length (norm).
  inline T length() const
  {
    return std::sqrt(this->lengthSquared());
  }

  /// Compound addition operator overload.
  inline Vector3D& operator+=(const Vector3D<T>& v)
  {
    xp += v.xp;
    yp += v.yp;
    yp += v.zp;
    return *this;
  }

  /// Compound subtraction operator overload.
  inline Vector3D<T>& operator-=(const Vector3D<T>& v)
  {
    xp -= v.xp;
    yp -= v.yp;
    yp -= v.zp;
    return *this;
  }

  /// Compound scalar multiplication operator overload.
  inline Vector3D<T>& operator*=(T scalar)
  {
    xp *= scalar;
    yp *= scalar;
    zp *= scalar;
    return *this;
  }

  /// Compound multiplication operator overload.
  inline Vector3D& operator*=(const Vector3D<T>& v)
  {
    xp *= v.xp;
    yp *= v.yp;
    zp *= v.zp;
    return *this;
  }

  /// Compound scalar division operator overload.
  inline Vector3D& operator/=(T divisor)
  {
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    return *this;
  }

  /// Unary negate operator overload.
  inline friend const Vector3D<T> operator-(const Vector3D<T>& v)
  {
    return Vector3D<T>(-v.xp, -v.yp, -v.zp);
  }

  /// Addition operator overload.
  inline friend const Vector3D<T> operator+(const Vector3D<T>& u,
                                            const Vector3D<T>& v)
  {
    return Vector3D<T>(u.xp + v.xp, u.yp + v.yp, u.zp + v.zp);
  }

  /// Subtraction operator overload.
  inline friend const Vector3D<T> operator-(const Vector3D<T>& u,
                                            const Vector3D<T>& v)
  {
    return Vector3D<T>(u.xp - v.xp, u.yp - v.yp, u.zp - v.zp);
  }

  /// Scalar multiplication operator overload.
  inline friend const Vector3D<T> operator*(T scalar, const Vector3D<T>& v)
  {
    return Vector3D<T>(v.xp * scalar, v.yp * scalar, v.zp * scalar);
  }

  /// Scalar multiplication operator overload.
  inline friend const Vector3D<T> operator*(const Vector3D<T>& v, T scalar)
  {
    return Vector3D<T>(v.xp * scalar, v.yp * scalar, v.zp * scalar);
  }

  /// Multiplication operator overload.
  inline friend const Vector3D<T> operator*(const Vector3D<T>& u,
                                            const Vector3D<T>& v)
  {
    return Vector3D<T>(u.xp * v.xp, u.yp * v.yp, u.zp * v.zp);
  }

  /// Normalize vector in place.
  inline void normalize()
  {
    *this /= this->length();
  }

  /// Returns a normalized vector from this vector.
  inline Vector3D<T> normalized()
  {
    return Vector3D<T>(*this).normalize();
  }

  /// Returns dot product of two vectors. [static]
  static inline T dotProduct(const Vector3D<T>& u, const Vector3D<T>& v)
  {
    return u.xp * v.xp + u.yp * v.yp + u.zp * v.zp;
  }

  /// Returns cross product of two vectors. [static]
  static inline Vector3D<T> crossProduct(const Vector3D<T>& u,
                                         const Vector3D<T>& v)
  {
    return Vector3D<T>(u.yp * v.zp - u.zp * v.yp,
                       u.zp * v.xp - u.xp * v.zp,
                       u.xp * v.yp - u.yp * v.xp);
  }

  /// Rotate around X axis.
  inline void rotateX(double alpha)
  {
    yp = std::cos(alpha) * yp - std::sin(alpha) * zp;
    zp = std::sin(alpha) * yp + std::cos(alpha) * zp;
  }

  /// Rotate around Y axis.
  inline void rotateY(double alpha)
  {
    xp = std::cos(alpha) * xp + std::sin(alpha) * zp;
    zp = std::cos(alpha) * zp - std::sin(alpha) * xp;
  }

  /// Rotate around Z axis.
  inline void rotateZ(double alpha)
  {
    xp = std::cos(alpha) * xp - std::sin(alpha) * yp;
    yp = std::sin(alpha) * xp + std::cos(alpha) * yp;
  }

  /// Transform with a matrix. (scale, rotate)
  inline void transform(const Matrix<T>& m)
  {
    T oldX = xp;
    T oldY = yp;
    T oldZ = zp;
    xp = m(0, 0) * oldX + m(0, 1) * oldY + m(0, 2) * oldZ;
    yp = m(1, 0) * oldX + m(1, 1) * oldY + m(1, 2) * oldZ;
    zp = m(2, 0) * oldX + m(2, 1) * oldY + m(2, 2) * oldZ;
  }

}; // end class Vector3D

} // end namespace Utils
