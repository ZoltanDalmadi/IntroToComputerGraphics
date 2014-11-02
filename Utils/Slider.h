#pragma once

#include <GL/glut.h>
#include "Point2D.h"
#include "Line.h"

namespace Utils
{

class Slider
{
private:
  Line<GLint> body;
  Point2D<GLint> handle;
  int value = 0;

  GLfloat lineWidth = 2.0;
  Color bodyColor = BLACK;
  GLfloat handleSize = 12.0;
  Color handleColor = DARK_GREEN;

  inline void init()
  {
    this->body.color = this->bodyColor;
    this->body.lineWidth = this->lineWidth;
    this->handle.color = this->handleColor;
    this->handle.size = this->handleSize;
  }

public:
  Slider(GLint x1, GLint y1, GLint x2, GLint y2)
    : body(x1, y1, x2, y2), handle(x1, y1)
  {
    init();
  }

  Slider(Point2D<GLint> a, Point2D<GLint> b)
    : body(a, b), handle(a)
  {
    init();
  }

  inline void setHandleSize(GLfloat size)
  {
    this->handle.size = size;
  }

  inline void setHandleColor(Color color)
  {
    this->handle.color = color;
  }

  inline void setValue(int val)
  {
    double x = std::round(this->body.x1() + body.dx() *
                          (static_cast<double>(val) / 100));
    this->setHandlePos(static_cast<int>(x));
  }

  inline int getValue() const
  {
    return this->value;
  }

  inline void updateValue()
  {
    double x =
      std::round(static_cast<double>(handle.x() - body.x1()) / body.dx() * 100);

    value = static_cast<int>(x);
  }

  void setHandlePos(GLint pos)
  {
    GLint x1 = body.x1();
    GLint x2 = body.x2();

    if (pos >= x1 && pos <= x2)
    {
      this->handle.setX(pos);

      if (pos < x1)
        this->handle.setX(x1);

      if (pos > x2)
        this->handle.setX(x2);

      updateValue();
    }
  }

  inline void checkClick(const Point2D<GLint>& mousePos, int sens)
  {
    this->handle.checkClick(mousePos, sens);
  }

  inline void checkClick(GLint xMouse, GLint yMouse, int sens)
  {
    Point2D<GLint> mousePos(xMouse, yMouse);
    this->handle.checkClick(mousePos, sens);
  }

  inline bool isDragging() const
  {
    return this->handle.clicked;
  }

  inline void release()
  {
    this->handle.release();
  }

  inline void draw()
  {
    this->body.draw();
    this->handle.draw();
  }

  inline void drawBody()
  {
    this->body.draw();
  }

  inline void drawHandle()
  {
    this->handle.draw();
  }

}; // end class Slider

} // end namespace Utils
