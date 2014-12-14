#pragma once

#include "Color.h"
#include "Matrix.h"
#include "Point2D.h"

namespace Utils
{

template <typename T> class Point2D;

template <typename T>
class Point3D
{
private:
  // Coordinates.
  T xp;
  T yp;
  T zp;

public:
  Color color = BLACK;
  GLfloat size = 10.0;

  Point3D() : xp(0), yp(0), zp(0)
  {
  }

  Point3D(T x, T y, T z) : xp(x), yp(y), zp(z)
  {
  }

  virtual ~Point3D()
  {
  }

  /// Returns X coordinate.
  inline T x() const
  {
    return xp;
  }

  /// Returns Y coordinate.
  inline T y() const
  {
    return yp;
  }

  /// Returns Z coordinate.
  inline T z() const
  {
    return zp;
  }

  /// Sets X coordinate.
  inline void setX(T value)
  {
    xp = value;
  }

  /// Sets Y coordinate.
  inline void setY(T value)
  {
    yp = value;
  }

  /// Sets Z coordinate.
  inline void setZ(T value)
  {
    zp = value;
  }

  /// Sets all coordinates.
  inline void setXYZ(T xValue, T yValue, T zValue)
  {
    xp = xValue;
    yp = yValue;
    zp = zValue;
  }

  /// Sets all coordinates from an existing point.
  inline void setXYZ(const Point3D<T>& p)
  {
    xp = p.x();
    yp = p.y();
    zp = p.z();
  }

  /// Compound addition operator overload.
  inline Point3D<T>& operator+=(const Point3D<T>& p)
  {
    xp += p.xp;
    yp += p.yp;
    zp += p.zp;
    return *this;
  }

  /// Compound subtraction operator overload.
  inline Point3D<T>& operator-=(const Point3D<T>& p)
  {
    xp -= p.xp;
    yp -= p.yp;
    zp -= p.zp;
    return *this;
  }

  /// Compound multiplication operator overload.
  inline Point3D<T>& operator*=(T c)
  {
    xp *= c;
    yp *= c;
    zp *= c;
    return *this;
  }

  /// Compound division operator overload.
  inline Point3D<T>& operator/=(T divisor)
  {
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    return *this;
  }

  /// Convert to 2D coordinates.
  inline Point2D<T> normalized() const
  {
    return Point2D<T>(xp / zp, yp / zp);
  }

}; // end class Point3D

template <typename T>
class Point3DH
{
private:
  T xp;
  T yp;
  T zp;
  T wp;

public:
  Point3DH() : xp(0.0f), yp(0.0f), zp(0.0f), wp(1.0f) {}
  Point3DH(T x, T y, T z, T w = 1.0f) : xp(x), yp(y), zp(z), wp(w) {}
  Point3DH(const Point3D<T>& p) : xp(p.x()), yp(p.y()), zp(p.z()), wp(1.0f) {}

  /// Returns X coordinate.
  inline T x() const
  {
    return xp;
  }

  /// Returns Y coordinate.
  inline T y() const
  {
    return yp;
  }

  /// Returns Z coordinate.
  inline T z() const
  {
    return zp;
  }

  /// Returns W coordinate.
  inline T w() const
  {
    return wp;
  }

  /// Sets X coordinate.
  inline void setX(T x)
  {
    this->xp = x;
  }

  /// Sets Y coordinate.
  inline void setY(T y)
  {
    this->yp = y;
  }

  /// Sets Z coordinate.
  inline void setZ(T z)
  {
    this->zp = z;
  }

  /// Sets W coordinate.
  inline void setW(T w)
  {
    this->wp = w;
  }

  /// Convert to normal coordinates.
  inline Point3D<T> normalized() const
  {
    return Point3D<T>(xp / wp, yp / wp, zp / wp);
  }

  /// Convert to normal 2D coordinates.
  inline Point2D<T> normalized2D() const
  {
    return Point2D<T>(xp / wp, yp / wp);
  }

  inline void transform(const Matrix<T>& m)
  {
    T oldX = xp;
    T oldY = yp;
    T oldZ = zp;
    T oldW = wp;
    xp = m(0, 0) * oldX + m(0, 1) * oldY + m(0, 2) * oldZ + m(0, 3) * oldW;
    yp = m(1, 0) * oldX + m(1, 1) * oldY + m(1, 2) * oldZ + m(1, 3) * oldW;
    zp = m(2, 0) * oldX + m(2, 1) * oldY + m(2, 2) * oldZ + m(2, 3) * oldW;
    wp = m(3, 0) * oldX + m(3, 1) * oldY + m(3, 2) * oldZ + m(3, 3) * oldW;
  }

  inline Point3DH<T> transformed(const Matrix<T>& m) const
  {
    T oldX = xp;
    T oldY = yp;
    T oldZ = zp;
    T oldW = wp;
    return Point3DH<T>(
             m(0, 0) * oldX + m(0, 1) * oldY + m(0, 2) * oldZ + m(0, 3) * oldW,
             m(1, 0) * oldX + m(1, 1) * oldY + m(1, 2) * oldZ + m(1, 3) * oldW,
             m(2, 0) * oldX + m(2, 1) * oldY + m(2, 2) * oldZ + m(2, 3) * oldW,
             m(3, 0) * oldX + m(3, 1) * oldY + m(3, 2) * oldZ + m(3, 3) * oldW
           );
  }

}; // end class Point3DH

} // end namespace Utils
