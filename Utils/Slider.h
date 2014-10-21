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
  double value;

  GLfloat lineWidth = 2.0;
  Color bodyColor = BLACK;
  GLfloat handleSize = 12.0;
  Color handleColor = DARK_GREEN;

  inline void init() {
    this->body.color = this->bodyColor;
    this->handle.color = this->handleColor;
    value = 0.5;
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

  inline void updateValue() {
    this->value = static_cast<double>((this->handle.x() - this->body.x1()))
      / this->body.dx();
  }

  inline void setHandlePos(T pos) {
    if(pos >= body.x1() && pos <= body.x2()) {
      this->handle.setX(pos);
      updateValue();
    }
  }

  inline void checkClick(const Point2D<T>& mousePos, int sens) {
    this->handle.checkClick(mousePos, sens);
  }

  inline void checkClick(GLint& xMouse, GLint& yMouse, int sens) {
    Point2D<T> mousePos(static_cast<T>(xMouse),
                        static_cast<T>(yMouse));
    this->handle.checkClick(mousePos, sens);
  }

  inline void onDrag() {
    if(this->handle.clicked)
      setHandlePos(mouseX);
  }

  inline bool isDragging() {
    return this->handle.clicked;
  }

  inline void release() {
    this->handle.release();
  }

  inline void draw() {
    this->body.draw();
    this->handle.draw();
  }

}; // end class Slider

} // end namespace Utils