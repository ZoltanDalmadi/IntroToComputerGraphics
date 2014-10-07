#pragma once
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
  inline Circle(T cx, T cy, T r) : Ellipse(cx, cy, r, r) {}
  inline Circle(Point2D<T> c, T r) : Ellipse(c, r, r) {}

  virtual ~Circle() {}

  inline T getRadius() const {
    return rx;
  };

  inline void setRadius(T radius) {
    rx = radius;
    ry = radius;
  }

  /// Draw Circle with OpenGL calls.
  virtual void draw() const {
    glLineWidth(lineWidth);
    color.setGLColor();

    double gap = 2 * PI / points;
    double c = cos(gap); //precalculate the sine and cosine
    double s = sin(gap);
    double temp;
    double x = rx; //we start at angle = 0 
    double y = 0;

    if(filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for(size_t i = 0; i < points; i++) {
      Point2D<T> point(static_cast<T>(x + centre.x()), static_cast<T>(y + centre.y()));
      glVertex2<T>(point);

      //apply the rotation matrix
      temp = x;
      x = c * x - s * y;
      y = s * temp + c * y;
    }
    glEnd();
  }

  virtual void drawPoints() const {
    this->drawPoints(points);
  }

  virtual void drawPoints(size_t n) const {
    glPointSize(pointSize);
    pointColor.setGLColor();

    double gap = 2 * PI / n;
    double c = cos(gap); //precalculate the sine and cosine
    double s = sin(gap);
    double temp;
    double x = rx; //we start at angle = 0 
    double y = 0;

    glBegin(GL_POINTS);

    for(size_t i = 0; i < n; i++) {
      Point2D<T> point(static_cast<T>(x + centre.x()), static_cast<T>(y + centre.y()));
      glVertex2<T>(point); //output vertex 

      //apply the rotation matrix
      temp = x;
      x = c * x - s * y;
      y = s * temp + c * y;
    }

    glEnd();
  }

}; // end class Circle

} // endl namespace Utils
