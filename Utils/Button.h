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
  size_t paddingX = 10;
  size_t paddingY = 10;
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

  inline size_t getPaddingY() const
  {
    return paddingY;
  }

  inline void setPaddingY(const size_t &value)
  {
    paddingY = value;
  }

  inline size_t getPaddingX() const
  {
    return paddingX;
  }

  inline void setPaddingX(const size_t &value)
  {
    paddingX = value;
  }

  inline bool hover(const GLint& x, const GLint& y) const
  {
    return body.isInside(x, y);
  }

  void draw() const
  {
    body.draw();
    color.setGLColor();

    glRasterPos2i(body.left() + paddingX, body.bottom() + paddingY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)label.c_str());
  }
};

// end class Button

} // end namespace Utils
