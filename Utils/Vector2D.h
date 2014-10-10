#pragma once

namespace Utils {

template <typename T>
class Vector2D {
private:
  T xp, yp;

public:

  inline Vector2D(T x, T y) : xp(x), yp(y) {}

  virtual ~Vector2D() {}

  /// Returns X component.
  inline T x() const {
    return xp;
  }

  /// Returns Y component.
  inline T y() const {
    return yp;
  }

  /// Compound addition operator overload.
  inline Vector2D& operator+=(const Vector2D<T> &v) {
    xp += v.xp;
    yp += v.yp;
    return *this;
  }

  /// Compound subtraction operator overload.
  inline Vector2D<T>& operator-=(const Vector2D<T>& v) {
    xp -= v.xp;
    yp -= v.yp;
    return *this;
  }

  /// Compound scalar multiplication operator overload.
  inline Vector2D<T>& operator*=(T scalar) {
    xp *= scalar;
    yp *= scalar;
    return *this;
  }

  /// Compound multiplication operator overload.
  inline Vector2D& operator*=(const Vector2D<T> &v) {
    xp *= v.xp;
    yp *= v.yp;
    return *this;
  }

  /// Compound scalar division operator overload.
  inline Vector2D& operator/=(T divisor) {
    xp /= divisor;
    yp /= divisor;
    return *this;
  }

  /// Unary negate operator overload.
  inline friend const Vector2D<T> operator-(const Vector2D<T> &v) {
    return Vector2D<T>(-v.xp, -v.yp);
  }

  /// Addition operator overload.
  inline friend const Vector2D<T> operator+(const Vector2D<T> &u, const Vector2D<T> &v) {
    return Vector2D<T>(u.xp + v.xp, u.yp + v.yp);
  }

  /// Subtraction operator overload.
  inline friend const Vector2D<T> operator-(const Vector2D<T> &u, const Vector2D<T> &v) {
    return Vector2D<T>(u.xp - v.xp, u.yp - v.yp);
  }

  /// Scalar multiplication operator overload.
  inline friend const Vector2D<T> operator*(T scalar, const Vector2D<T> &v) {
    return Vector2D<T>(v.xp * scalar, v.yp * scalar);
  }

  /// Scalar multiplication operator overload.
  inline friend const Vector2D<T> operator*(const Vector2D<T> &v, T scalar) {
    return Vector2D<T>(v.xp * scalar, v.yp * scalar);
  }

  /// Multiplication operator overload.
  inline friend const Vector2D<T> operator*(const Vector2D<T> &u, const Vector2D<T> &v) {
    return Vector2D<T>(u.xp * v.xp, u.yp * v.yp);
  }

  /// Returns dot product of two vectors. [static]
  static inline T dotProduct(const Vector2D<T> &u, const Vector2D<T> &v) {
    return u.x() * v.x() + u.y() * v.y();
  }
  
  static inline Vector2D<T> reflectTo(const Vector2D<T> &u, const Vector2D<T> &v) {

  }

}; // end class Vector2D

}
