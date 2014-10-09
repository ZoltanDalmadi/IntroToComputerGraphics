#pragma once
#include "Ellipse.h"

namespace Utils {

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
      pointsVector.emplace_back(static_cast<T>(x + centre.x()),
                                static_cast<T>(y + centre.y()));

      //apply the rotation matrix
      temp = x;
      x = c * x - s * y;
      y = s * temp + c * y;
    }
  }

}; // end class Circle

} // endl namespace Utils
