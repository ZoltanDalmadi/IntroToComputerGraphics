#pragma once

#include <GL/freeglut.h>
#include "Point2D.h"
#include "Color.h"

namespace Utils
{

template <typename T>
class Rectangle
{
private:
  Point2D<T> topLeft;
  Point2D<T> topRight;
  Point2D<T> bottomRight;
  Point2D<T> bottomLeft;

public:
  Color color = RED;

  Rectangle(const Point2D<T>& topleft, const Point2D<T>& topright,
            const Point2D<T>& bottomright, const Point2D<T>& bottomleft)
    : topLeft(topleft), topRight(topright),
      bottomRight(bottomright), bottomLeft(bottomleft) {}

  Rectangle(T x, T y, T width, T height)
    : topLeft(x, y), topRight(x + width, y),
      bottomRight(x + width, y - height), bottomLeft(x, y - height) {}

  virtual ~Rectangle() {}

  inline T width()
  {
    return bottomRight.x() - topLeft.x();
  }

  inline T height()
  {
    return bottomRight.y() - topLeft.y();
  }

  void draw()
  {
    this->color.setGLColor();
    glBegin(GL_POLYGON);
    glVertex2<T>(topLeft);
    glVertex2<T>(topRight);
    glVertex2<T>(bottomRight);
    glVertex2<T>(bottomLeft);
    glEnd();
  }

}; // end class Rectangle

} // end namespace Utils
