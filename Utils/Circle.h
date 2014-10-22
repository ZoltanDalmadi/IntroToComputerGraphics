#pragma once

#include "Ellipse.h"

namespace Utils {

template <typename T>
class Circle : public Ellipse <T> {
 private:
  // hide unneeded methods from base class
  inline void setRadiusX(T radius);
  inline void setRadiusY(T radius);
  inline T getRadiusX() const;
  inline T getRadiusY() const;

 public:
  inline Circle(T cx, T cy, T r) : Ellipse<T>(cx, cy, r, r) {}
  inline Circle(T cx, T cy, T r, size_t points) : Ellipse<T>(cx, cy, r, r, points) {}
  inline Circle(Point2D<T> c, T r) : Ellipse<T>(c, r, r) {}
  virtual ~Circle() {}

  inline T getRadius() const {
    return this->rx;
  };

  inline void setRadius(T radius) {
    this->rx = radius;
    this->ry = radius;
  }

  /// Recalculate points (more efficient algorithm, but only for circles).
  virtual void recalcPoints() {
    this->pointsContainer.clear();
    double gap = 2 * PI / this->points;
    double c = cos(gap); //precalculate the sine and cosine
    double s = sin(gap);
    double temp;
    double x = this->rx; //we start at angle = 0
    double y = 0;

    for (size_t i = 0; i < this->points; i++) {
      this->pointsContainer.emplace_back(static_cast<T>(x), static_cast<T>(y));

      //apply the rotation matrix
      temp = x;
      x = c * x - s * y;
      y = s * temp + c * y;
    }
  }

}; // end class Circle

} // end namespace Utils
