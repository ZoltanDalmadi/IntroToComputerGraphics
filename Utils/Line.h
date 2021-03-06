#pragma once

#include <GL/freeglut.h>
#include "Point2D.h"

namespace Utils
{

template <typename T>
class Line
{
private:
  // Endpoints.
  Point2D<T> pt1;
  Point2D<T> pt2;

public:
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 10.0;
  Color color = BLACK;
  Color pointsColor = RED;

  /// Init Line with raw coordinates.
  inline Line(T a1, T a2, T b1, T b2) : pt1(a1, a2), pt2(b1, b2) {}

  /// Init Line with existing points.
  inline Line(Point2D<T> a, Point2D<T> b) : pt1(a), pt2(b) {}

  /// Returns starting point's X coordinate.
  inline T x1() const
  {
    return pt1.x();
  }

  /// Returns ending point's X coordinate.
  inline T x2() const
  {
    return pt2.x();
  }

  /// Returns starting point's Y coordinate.
  inline T y1() const
  {
    return pt1.y();
  }

  /// Returns ending point's Y coordinate.
  inline T y2() const
  {
    return pt2.y();
  }

  /// Returns starting point of Line.
  inline Point2D<T> p1() const
  {
    return pt1;
  }

  /// Returns ending point of Line.
  inline Point2D<T> p2() const
  {
    return pt2;
  }

  /// Returns a pointer to starting point of Line.
  inline Point2D<T> *pp1()
  {
    return pt1.getPointer();
  }

  /// Returns a pointer to ending point of Line.
  inline Point2D<T> *pp2()
  {
    return pt2.getPointer();
  }

  /// Returns a reference to starting point of Line.
  inline Point2D<T>& rp1()
  {
    return pt1;
  }

  /// Returns a reference to ending point of Line.
  inline Point2D<T>& rp2()
  {
    return pt2;
  }

  /// Set points.
  inline void setPoints(const Point2D<T>& P1, const Point2D<T>& P2)
  {
    pt1 = P1;
    pt2 = P2;
  }

  /// Set points with raw coordinates.
  inline void setPoints(T X1, T Y1, T X2, T Y2)
  {
    pt1 = Point2D<T>(X1, Y1);
    pt2 = Point2D<T>(X2, Y2);
  }

  /// Sets starting point.
  inline void setP1(const Point2D<T>& P1)
  {
    pt1 = P1;
  }

  /// Sets starting point. (overload)
  inline void setP1(T x, T y)
  {
    pt1.setX(x);
    pt1.setY(y);
  }

  /// Sets ending point.
  inline void setP2(const Point2D<T>& P2)
  {
    pt2 = P2;
  }

  /// Sets ending point. (overload)
  inline void setP2(T x, T y)
  {
    pt2.setX(x);
    pt2.setY(y);
  }

  /// Translate line with a given point.
  inline void translate(const Point2D<T>& point)
  {
    pt1 += point;
    pt2 += point;
  }

  /// Translate line with raw coordinates.
  inline void translate(T adx, T ady)
  {
    this->translate(Point2D<T>(adx, ady));
  }

  /// Returns horizontal component of Line's vector.
  inline T dx() const
  {
    return x2() - x1();
  }

  /// Returns vertical component of Line's vector.
  inline T dy() const
  {
    return y2() - y1();
  }

  /// Returns Line's length.
  inline T length() const
  {
    return Point2D<T>::distance(pt1, pt2);
  }

  /// Returns Line's length squared.
  inline T lengthSquared() const
  {
    return Point2D<T>::distance2(pt1, pt2);
  }

  /// Returns Point at position between pt1 and pt2 in [0, 1] range.
  inline Point2D<T> pointAt(T t) const
  {
    return Point2D<T>(x1() + dx() * t, y1() + dy() * t);
  }

  /// Returns Point at position between pt1 and pt2 in [0, 1] range. [static]
  static inline Point2D<T> pointAt(T t, const Point2D<T> &p1, const Point2D<T> &p2)
  {
    return Point2D<T>(p1.x() + (p2.x() - p1.x()) * t,
                      p1.y() + (p2.y() - p1.y()) * t);
  }

  /// Returns a perpendicular line with the same starting point and length.
  inline Line<T> normalVector() const
  {
    return Line<T>(pt1, pt1 + Point2D<T>(-dy(), dx()));
  }

  /// Standard equation of Line.
  /// If returned value = 0, Point is on line.
  /// If returned value > 0, Point is on half plane as normalvector is.
  /// If returned value < 0, Point is on half plane opposite to normalvector.
  inline T standardEquation(T x, T y) const
  {
    T DX = dx();
    T DY = dy();
    T C = DY * x1() - DX * y1();
    return -DY * x + DX * y + C;
  }

  /// Standard equation of Line overload.
  inline T standardEquation(const Point2D<T>& p) const
  {
    return this->standardEquation(p.x(), p.y());
  }

  /// Standard equation of Line overload. [static]
  /// p1 and p2 defines a line, p3 is the point to check.
  static inline T standardEquation(const Point2D<T>& p1,
                                   const Point2D<T>& p2,
                                   const Point2D<T>& p3)
  {
    T DX = p2.x() - p1.x();
    T DY = p2.y() - p1.y();
    T C = DY * p1.x() - DX * p1.y();
    return -DY * p3.x() + DX * p3.y() + C;
  }

  /// Get line's parameters.
  inline Point2DH<T> getParams() const
  {
    return Point2DH<T>(-dy(), dx(), x1() * y2() - x2() * y1());
  }

  /// Get line's parameters. [static]
  static inline Point2DH<T> getParams(const Point2D<T>& u, const Point2D<T>& v)
  {
    return Point2DH<T> (u.y() - v.y(),
                        v.x() - u.x(),
                        u.x() * v.y() - v.x() * u.y());
  }

  inline Point2D<T> computeIntersectionPoint(const Line<T>& other) const
  {
    return Line::computeIntersectionPoint(this->getParams(), other.getParams());
  }

  static inline Point2D<T> computeIntersectionPoint(const Point2DH<T>& u,
                                                    const Point2DH<T>& v)
  {
    return Point2DH<T>(u.y() * v.w() - v.y() * u.w(),
                       v.x() * u.w() - u.x() * v.w(),
                       u.x() * v.y() - v.x() * u.y()).normalized();
  }

  inline bool isPointLeft(T x, T y) const
  {
    return (y - y1())*(x2() - x1()) > (x - x1())*(y2() - y1());
  }

  inline bool isPointLeft(const Point2D<T>& p) const
  {
    return this->isPointLeft(p.x(), p.y());
  }

  static inline bool isPointLeft(T x, T y,
                                 const Point2D<T>& p1, const Point2D<T>& p2)
  {
    return (y - p1.y())*(p2.x() - p1.x()) > (x - p1.x())*(p2.y() - p1.y());
  }

  static inline bool isPointLeft(const Point2D<T>& u,
                                 const Point2D<T>& p1, const Point2D<T>& p2)
  {
    return isPointLeft(u.x(), u.y(), p1, p2);
  }

  /// Returns distance of Point2D to Line. [static]
  static inline T pDistanceToLine(const Point2D<T>& p, const Line<T>& line)
  {
    T DX = line.dx();
    T DY = line.dy();
    return std::abs(line.standardEquation(p.x(), p.y())) / sqrt(DY * DY + DX * DX);
  }

  /// Returns distance of Point2D to Line. [static]
  static inline T pDistanceToLineSquared(const Point2D<T>& p,
                                         const Line<T>& line)
  {
    T DX = line.dx();
    T DY = line.dy();
    T e = line.standardEquation(p.x(), p.y());
    return e * e / (DY * DY + DX * DX);
  }

  /// Returns dot product of two lines interpreted as vectors. [static]
  static inline T dotProduct(const Line<T>& l1, const Line<T>& l2)
  {
    return l1.dx() * l2.dx() + l1.dy() * l2.dy();
  }

  /// Draw Line with OpenGL calls.
  void draw() const
  {
    glLineWidth(lineWidth);
    color.setGLColor();
    glBegin(GL_LINES);
    glVertex2<T>(pt1);
    glVertex2<T>(pt2);
    glEnd();
  }

  /// Draw Line's endpoints with OpenGL calls.
  void drawPoints() const
  {
    glPointSize(pointSize);
    pointsColor.setGLColor();
    glBegin(GL_POINTS);
    glVertex2<T>(pt1);
    glVertex2<T>(pt2);
    glEnd();
  }

  /// Prints line to a stream.
  friend inline std::ostream& operator<<(std::ostream& os, const Line<T>& line)
  {
    return os << "[" << line.p1() << " : " << line.p2() << "]" << std::endl;
  }

  /// Unary negate operator overload. Flips line direction.
  inline friend Line<T> operator-(const Line<T>& line)
  {
    return Line<T>(line.x1(), line.y1(),
                   line.x2() - 2 * line.dx(),
                   line.y2() - 2 * line.dy());
  }

}; // end class Line

} // end namespace Utils
