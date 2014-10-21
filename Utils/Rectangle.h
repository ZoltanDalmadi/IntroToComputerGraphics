#pragma once

#include <GL/freeglut.h>
#include "Point2D.h"
#include "Color.h"

namespace Utils {

template <typename T>
class Rectangle {
private:
  Point2D<T> topLeft;
  Point2D<T> bottomRight;

public:
  Rectangle(const Point2D<T> &topleft, const Point2D<T> &bottomRight)
    : topLeft(topleft), bottomRight(bottomRight) {}

  Rectangle(T x, T y, T width, T height)
    : topLeft(x, y), bottomRight(x + width, y + height) {}

  virtual ~Rectangle() {}

  inline T width() {
    return bottomRight.x() - topLeft.x();
  }

  inline T height() {
    return bottomRight.y() - topLeft.y();
  }

}; // end class Rectangle

} // end namespace Utils
