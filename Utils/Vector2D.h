#pragma once
#include <cmath>
//#include "Line.h"
#include "Point2D.h"
#include "Color.h"

namespace Utils
{
template <typename T> class Point2D;

template <typename T>
class Vector2D
{
private:
  T xp, yp;

public:
  GLfloat lineWidth = 2.0;
  Color color = BLACK;

  inline Vector2D(T x, T y) : xp(x), yp(y) {}

  virtual ~Vector2D() {}

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

  /// Sets components.
  inline void set(T x, T y)
  {
    xp = x;
    yp = y;
  }

  /// Sets components. (overload)
  inline void set(const Vector2D<T>& v)
  {
    xp = v.xp;
    yp = v.yp;
  }

  /// Returns length (norm).
  inline T length() const
  {
    return std::hypot(xp, yp);
  }

  /// Returns length (norm) squared.
  inline T lengthSquared() const
  {
    return xp * xp + yp * yp;
  }

  /// Compound addition operator overload.
  inline Vector2D& operator+=(const Vector2D<T>& v)
  {
    xp += v.xp;
    yp += v.yp;
    return *this;
  }

  /// Compound subtraction operator overload.
  inline Vector2D<T>& operator-=(const Vector2D<T>& v)
  {
    xp -= v.xp;
    yp -= v.yp;
    return *this;
  }

  /// Compound scalar multiplication operator overload.
  inline Vector2D<T>& operator*=(T scalar)
  {
    xp *= scalar;
    yp *= scalar;
    return *this;
  }

  /// Compound multiplication operator overload.
  inline Vector2D& operator*=(const Vector2D<T>& v)
  {
    xp *= v.xp;
    yp *= v.yp;
    return *this;
  }

  /// Compound scalar division operator overload.
  inline Vector2D& operator/=(T divisor)
  {
    xp /= divisor;
    yp /= divisor;
    return *this;
  }

  /// Unary negate operator overload.
  inline friend const Vector2D<T> operator-(const Vector2D<T>& v)
  {
    return Vector2D<T>(-v.xp, -v.yp);
  }

  /// Addition operator overload.
  inline friend const Vector2D<T> operator+(const Vector2D<T>& u,
                                            const Vector2D<T>& v)
  {
    return Vector2D<T>(u.xp + v.xp, u.yp + v.yp);
  }

  /// Subtraction operator overload.
  inline friend const Vector2D<T> operator-(const Vector2D<T>& u,
                                            const Vector2D<T>& v)
  {
    return Vector2D<T>(u.xp - v.xp, u.yp - v.yp);
  }

  /// Scalar multiplication operator overload.
  inline friend const Vector2D<T> operator*(T scalar, const Vector2D<T>& v)
  {
    return Vector2D<T>(v.xp * scalar, v.yp * scalar);
  }

  /// Scalar multiplication operator overload.
  inline friend const Vector2D<T> operator*(const Vector2D<T>& v, T scalar)
  {
    return Vector2D<T>(v.xp * scalar, v.yp * scalar);
  }

  /// Multiplication operator overload.
  inline friend const Vector2D<T> operator*(const Vector2D<T>& u,
                                            const Vector2D<T>& v)
  {
    return Vector2D<T>(u.xp * v.xp, u.yp * v.yp);
  }

  /// Returns dot product of two vectors. [static]
  static inline T dotProduct(const Vector2D<T>& u, const Vector2D<T>& v)
  {
    return u.xp * v.xp + u.yp * v.yp;
  }

  inline void reflectFrom(const Vector2D<T>& v)
  {
    T c = 2 * Vector2D<T>::dotProduct(v, *this) / v.lengthSquared();
    this->set(-(*this) + c * v);
  }

  static inline Vector2D<T> reflectFrom(const Vector2D<T>& u,
                                        const Vector2D<T>& v)
  {
    T c = 2 * Vector2D<T>::dotProduct(v, u) / v.lengthSquared();
    return Vector2D<T>(-u + c * v);
  }

  void draw(const Point2D<T>& p, T arrowLength, double arrowDeg) const
  {
    auto endPoint = p.translated(*this);

    double angle = std::atan2(yp, xp) + Utils::PI;
    Point2D<T> left(endPoint.x() + arrowLength * std::cos(angle - arrowDeg),
                    endPoint.y() + arrowLength * std::sin(angle - arrowDeg));

    Point2D<T> right(endPoint.x() + arrowLength * std::cos(angle + arrowDeg),
                     endPoint.y() + arrowLength * std::sin(angle + arrowDeg));

    glLineWidth(lineWidth);
    color.setGLColor();

    glBegin(GL_LINES);
    glVertex2<T>(p);
    glVertex2<T>(endPoint);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2<T>(endPoint);
    glVertex2<T>(left);
    glVertex2<T>(right);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2<T>(endPoint);
    glVertex2<T>(left);
    glVertex2<T>(right);
    glEnd();
  }

}; // end class Vector2D

} // end namespace Utils
