#pragma once

#include <GL/glut.h>
#include "Rectangle.h"

namespace Utils {

template <typename T>
class Slider {
private:
  Rectangle<T> handle;
  T handleWidth;
  T handleHeigh;
  T size;
  T x;
  T y;

public:
  bool vertical = false;
  GLfloat lineWidth = 1.0;
  Color color = BLACK;

  Slider(T x, T y, T siz, T handleW, T handleH)
    : handle(x, y, handleH, handleW), size(siz) {}
  virtual ~Slider() {}

  void setHandleSize() {
    this->handle.setSize();
  }

}; // end class Slider

} // end namespace Utils