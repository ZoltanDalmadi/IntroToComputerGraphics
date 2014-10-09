#pragma once
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include "Point2D.h"
#include "Color.h"

namespace Utils {

template <typename T>
class Ellipse {
protected:
  Point2D<T> centre;
  T rx;
  T ry;
  size_t points = 48;

public:
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 6.0;
  GLfloat centrePointSize = 10.0;
  std::vector < Point2D<T> > pointsVector;
  Color color = BLACK;
  Color pointColor = BLUE;
  Color centrePointColor = RED;
  bool filled = false;

  inline Ellipse(T cx, T cy, T rx, T ry) : centre(cx, cy), rx(rx), ry(ry) {}
  inline Ellipse(Point2D<T> c, T rx, T ry) : centre(c), rx(rx), ry(ry) {}

  virtual ~Ellipse() {}

  /// Returns center point's X coordinate.
  inline T cx() const {
    return centre.x();
  }

  /// Returns center point's Y coordinate.
  inline T cy() const {
    return centre.y();
  }

  /// Returns center point.
  inline Point2D<T> c() const {
    return centre;
  }

  /// Returns a pointer to center point.
  inline Point2D<T>* pc() {
    return centre.getPointer();
  }

  /// Returns a reference to center point.
  inline Point2D<T>& rc() {
    return centre;
  }

  /// Sets center point.
  inline void setCentre(const Point2D<T>& P) {
    centre = P;
  }

  /// Sets center point. (overload)
  inline void setCentre(T x, T y) {
    centre.setX(x);
    centre.setY(y);
  }

  /// Returns X radius.
  inline T getRadiusX() const {
    return rx;
  };

  /// Returns Y radius.
  inline T getRadiusY() const {
    return ry;
  };

  /// Sets X radius.
  inline void setRadiusX(T radius) {
    rx = radius;
  };

  /// Sets Y radius.
  inline void setRadiusY(T radius) {
    ry = radius;
  };

  /// Returns number of points.
  inline size_t getPoints() const {
    return points;
  }

  /// Sets number of points.
  inline void setPoints(size_t p) {
    points = p;
    recalcPoints();
  }

  /// Translate Ellipse with a given point.
  inline void translate(const Point2D<T>& point) {
    centre += point;
  }

  void operator++(int) {
    points++;
    recalcPoints();
  }

  void operator--(int) {
    points--;
    recalcPoints();
  }

  /// Translate Ellipse with raw coordinates.
  inline void translate(T adx, T ady) {
    this->translate(Point2D<T>(adx, ady));
  }

  /// recalculate points.
  virtual void recalcPoints() {}

  /// Draw Ellipse with OpenGL calls.
  virtual void draw() const {
    glLineWidth(lineWidth);
    color.setGLColor();

    double gap = 2 * PI / points;

    if(filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for(size_t i = 0; i <= points; ++i) {
      Point2D<T> point(static_cast<T>(centre.x() + rx*cos(i*gap)),
                       static_cast<T>(centre.y() + ry*sin(i*gap)));
      glVertex2<T>(point);
    }
    glEnd();
  }

  virtual inline void drawPoints() const {
    this->drawPoints(points);
  }

  virtual void drawPoints(size_t n) const {
    glPointSize(pointSize);
    pointColor.setGLColor();

    double gap = 2 * PI / n;
    glBegin(GL_POINTS);
    for(size_t i = 0; i <= n; ++i) {
      Point2D<T> point(static_cast<T>(centre.x() + rx*cos(i*gap)),
                       static_cast<T>(centre.y() + ry*sin(i*gap)));
      glVertex2<T>(point);
    }
    glEnd();
  }

  inline void drawCenterPoint() const {
    glPointSize(pointSize);
    centrePointColor.setGLColor();
    glBegin(GL_POINTS);
    glVertex2<T>(centre);
    glEnd();
  }

}; // end class Ellipse

} // end namespace Utils
