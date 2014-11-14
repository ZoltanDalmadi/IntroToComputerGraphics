#pragma once

#include <ostream>
#include <cmath>
#include "Color.h"
#include "Matrix.h"

namespace Utils
{

// Forward declare
template <typename T> class Point2D;
template <typename T> class Line;
template <typename T> void glVertex2(const Point2D<T>& p);

template <typename T>
class Point2D
{
private:
  // Coordinates.
  T xp;
  T yp;

public:
  Color color = BLACK;
  GLfloat size = 10.0;
  bool clicked = false;

  /// Initialize point at (0, 0) (origin). (glut: bottom-left corner)
  Point2D() : xp(0), yp(0) {}

  /// Initialize with raw coordinates.
  Point2D(T x, T y) : xp(x), yp(y) {}

  /// Returns true if Point's coordinates are zero.
  inline bool isNull() const
  {
    return (xp == 0) && (yp == 0);
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

  /// Sets X and Y coordinate.
  inline void setXY(T xValue, T yValue)
  {
    xp = xValue;
    yp = yValue;
  }

  /// Sets X and Y coordinate from an existing point.
  inline void setXY(const Point2D<T>& p)
  {
    xp = p.x();
    yp = p.y();
  }

  /// Changes X coordinate by an amount.
  inline void changeX(T value)
  {
    xp += value;
  }

  /// Changes Y coordinate by an amount.
  inline void changeY(T value)
  {
    yp += value;
  }

  /// Returns a pointer to this Point2D.
  inline Point2D<T> *getPointer()
  {
    return this;
  }

  /// Returns a reference to this Point2D.
  inline Point2D<T>& getRef()
  {
    return *this;
  }

  /// Compound addition operator overload.
  inline Point2D<T>& operator+=(const Point2D<T>& p)
  {
    xp += p.xp;
    yp += p.yp;
    return *this;
  }

  /// Compound subtraction operator overload.
  inline Point2D<T>& operator-=(const Point2D<T>& p)
  {
    xp -= p.xp;
    yp -= p.yp;
    return *this;
  }

  /// Compound multiplication operator overload.
  inline Point2D<T>& operator*=(T c)
  {
    xp *= c;
    yp *= c;
    return *this;
  }

  /// Compound division operator overload.
  inline Point2D<T>& operator/=(T divisor)
  {
    xp /= divisor;
    yp /= divisor;
    return *this;
  }

  /// Equality operator overload.
  friend inline bool operator==(const Point2D<T>& p1, const Point2D<T>& p2)
  {
    return (p2.xp - p1.xp < 0.001f) && (p2.yp - p1.yp < 0.001f);
  }

  /// Inequality operaton overload.
  friend inline bool operator!=(const Point2D<T>& p1, const Point2D<T>& p2)
  {
    return !(p2.xp - p1.xp < 0.001f) || !(p2.yp - p1.yp < 0.001f);
  }

  /// Addition operator overload.
  friend inline const Point2D<T> operator+(const Point2D<T>& p1,
                                           const Point2D<T>& p2)
  {
    return Point2D<T>(p1.xp + p2.xp, p1.yp + p2.yp);
  }

  /// Subtraction operator overload.
  friend inline const Point2D<T> operator-(const Point2D<T>& p1,
                                           const Point2D<T>& p2)
  {
    return Point2D<T>(p1.xp - p2.xp, p1.yp - p2.yp);
  }

  /// Scalar multiplication operator overload (Point on left side).
  friend inline const Point2D<T> operator*(const Point2D<T>& p, T c)
  {
    return Point2D<T>(p.xp * c, p.yp * c);
  }

  /// Scalar multiplication operator overload (Point on right side).
  friend inline const Point2D<T> operator*(T c, const Point2D<T>& p)
  {
    return Point2D<T>(p.xp * c, p.yp * c);
  }

  /// Unary positive operator overload.
  friend inline const Point2D<T> operator+(const Point2D<T>& p)
  {
    return p;
  }

  /// Unary negate operator overload.
  friend inline const Point2D<T> operator-(const Point2D<T>& p)
  {
    return Point2D<T>(-p.xp, -p.yp);
  }

  /// Division operator overload.
  friend inline const Point2D<T> operator/(const Point2D<T>& p, T divisor)
  {
    return Point2D<T>(p.xp / divisor, p.yp / divisor);
  }

  /// Stream redirection operator overload.
  friend inline std::ostream& operator<<(std::ostream& os, Point2D<T>& p)
  {
    return os << "(" << p.xp << ", " << p.yp << ")";
  }

  /// Returns distance between two points. [static]
  static inline T distance(const Point2D<T>& p1, const Point2D<T>& p2)
  {
    return std::hypot(p2.xp - p1.xp, p2.yp - p1.yp);
  }

  /// Returns distance squared between two points. [static]
  static inline T distance2(const Point2D<T>& p1, const Point2D<T>& p2)
  {
    T x = p2.xp - p1.xp;
    T y = p2.yp - p1.yp;
    return x * x + y * y;
  }

  /// Returns dot product of two points. [static]
  static inline T dotProduct(const Point2D<T>& p1, const Point2D<T>& p2)
  {
    return p1.xp * p2.xp + p1.yp * p2.yp;
  }

  /// Returns distance of Point2D to Line.
  inline T distanceToLine(const Line<T>& line)
  {
    T DX = line.dx();
    T DY = line.dy();
    return std::abs(line.standardEquation(xp, yp)) / sqrt(DY * DY + DX * DX);
  }

  /// Returns distance of Point2D to Line.
  inline T distanceToLineSquared(const Line<T>& line)
  {
    T DX = line.dx();
    T DY = line.dy();
    T e = line.standardEquation(xp, yp);
    return e * e / (DY * DY + DX * DX);
  }

  inline void checkClick(const Point2D<T>& mousePos, int sens)
  {
    int s = sens * sens;

    if (Point2D::distance2(*this, mousePos) < s)
    {
      this->clicked = true;
    }
  }

  inline void checkClick(GLint xMouse, GLint yMouse, int sens)
  {
    Point2D<T> mousePos(static_cast<T>(xMouse),
                        static_cast<T>(yMouse));
    this->checkClick(mousePos, sens);
  }

  inline void release()
  {
    this->clicked = false;
  }

  /// Draw point with OpenGL calls.
  void draw() const
  {
    glPointSize(size);
    color.setGLColor();
    glBegin(GL_POINTS);
    glVertex2<T>(*this);
    glEnd();
  }

  inline void transform(const Matrix<T>& m)
  {
    T oldX = xp;
    T oldY = yp;
    xp = m(0, 0) * oldX + m(0, 1) * oldY + m(0, 2);
    yp = m(1, 0) * oldX + m(1, 1) * oldY + m(1, 2);
  }

}; // end class Point2D

/// Type specific OpenGL calls.
template <> void glVertex2<GLshort>(const Point2D<GLshort>& p)
{
  glVertex2s(p.x(), p.y());
}

template <> void glVertex2<GLint>(const Point2D<GLint>& p)
{
  glVertex2i(p.x(), p.y());
}

template <> void glVertex2<GLfloat>(const Point2D<GLfloat>& p)
{
  glVertex2f(p.x(), p.y());
}

template <> void glVertex2<GLdouble>(const Point2D<GLdouble>& p)
{
  glVertex2d(p.x(), p.y());
}

} // end namespace Utils
