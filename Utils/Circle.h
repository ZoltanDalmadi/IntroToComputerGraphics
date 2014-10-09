#pragma once
#include <vector>
#include "Ellipse.h"

namespace Utils {

const GLdouble PI = 3.14159265358979323846;

template <typename T>
class Circle : public Ellipse < T > {
private:
  // hide unneeded methods from base class
  inline void setRadiusX(T radius);
  inline void setRadiusY(T radius);
  inline T getRadiusX() const;
  inline T getRadiusY() const;

public:
  inline Circle(T cx, T cy, T r) : Ellipse(cx, cy, r, r) {
    recalcPoints();
  }

  inline Circle(Point2D<T> c, T r) : Ellipse(c, r, r) {
    recalcPoints();
  }

  virtual ~Circle() {}

  inline T getRadius() const {
    return rx;
  };

  inline void setRadius(T radius) {
    rx = radius;
    ry = radius;
  }


  /// recalculate points.
  virtual void recalcPoints() {
    pointsVector.clear();
    double gap = 2 * PI / points;
    double c = cos(gap); //precalculate the sine and cosine
    double s = sin(gap);
    double temp;
    double x = rx; //we start at angle = 0 
    double y = 0;

    for(size_t i = 0; i < points; i++) {
      Point2D<T> point(static_cast<T>(x + centre.x()), static_cast<T>(y + centre.y()));
      pointsVector.push_back(point);

      //apply the rotation matrix
      temp = x;
      x = c * x - s * y;
      y = s * temp + c * y;
    }
  }

  /// Draw Circle with OpenGL calls.
  virtual void draw() const {
    glLineWidth(lineWidth);
    color.setGLColor();

    if(filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for(size_t i = 0; i < points; i++) {
      glVertex2<T>(pointsVector[i]);
    }
    glEnd();
  }

  virtual void drawPoints() const {
    this->drawPoints(points);
  }

  virtual void drawPoints(size_t n) const {
    glPointSize(pointSize);
    pointColor.setGLColor();

    glBegin(GL_POINTS);

    for(size_t i = 0; i < n; i++) {
      glVertex2<T>(pointsVector[i]); //output vertex 
    }

    glEnd();
  }

  void drawDiagonals() const {
    glLineWidth(lineWidth);
    color.setGLColor();

    glBegin(GL_LINES);
    for(size_t i = 0; i < points; i++) {
      for(size_t j = i + 1; j < points; j++) {
        glVertex2<T>(pointsVector[i]); //output vertex 
        glVertex2<T>(pointsVector[j]); //output vertex 
      }
    }
    glEnd();
  }

}; // end class Circle

} // endl namespace Utils
