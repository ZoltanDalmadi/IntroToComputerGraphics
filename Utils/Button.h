#pragma once

#include <GL/freeglut.h>
#include <string>
#include "Rectangle.h"

namespace Utils
{

class Button
{
private:
  Rectangle<GLint> body;
  std::string label;
  size_t labelPadding = 10;
  Color color = BLACK;

public:
  Button(std::string label, GLint x, GLint y, GLint width, GLint height)
    : body(x, y, x + width, y + height), label(label)
  {
    body.color = this->color;
  }

  inline void setColor(Color color)
  {
    this->color = color;
    this->body.color = color;
  }

  bool hover(const GLint& x, const GLint& y) const
  {
    return body.isInside(x, y);
  }

  void draw() const
  {
    body.draw();
    color.setGLColor();

    glRasterPos2i(body.left() + labelPadding, body.bottom() + labelPadding);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)label.c_str());
  }

}; // end class Button

} // end namespace Utils
