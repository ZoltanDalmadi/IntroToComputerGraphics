#pragma once

#include <GL/glut.h>
#include "Point2D.h"
#include "Line.h"

namespace Utils {

template <typename T>
class Slider {
private:
  Line<T> body;
  Point2D<T> handle;
  double value = 0.5;

  GLfloat lineWidth = 2.0;
  Color bodyColor = BLACK;
  GLfloat handleSize = 12.0;
  Color handleColor = DARK_GREEN;

  inline void init() {
    this->body.color = this->bodyColor;
    this->handle.color = this->handleColor;
    handle.setX(static_cast<T>(body.x1() + body.dx() * value));
  }

public:
  inline Slider(T x1, T y1, T x2, T y2) : body(x1, y2, x2, y2), handle(x1, y1) {
    init();
  }

  inline Slider(Point2D<T> a, Point2D<T> b)
    : body(a, b), handle(body.x1(), body.x2()) {
    init();
  }

  virtual ~Slider() {}

  inline void setValue(const double& val) {
    this->value = val;
  }

  inline double getValue() {
    return this->value;
  }

  inline void draw() {
    this->body.draw();
    this->handle.draw();
  }

}; // end class Slider

} // end namespace Utils