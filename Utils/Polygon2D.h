#pragma once

#include <GL/freeglut.h>
#include "Point2D.h"
#include "Color.h"

namespace Utils
{

template <typename T>
class Polygon2D
{
protected:
  size_t points = 0;

public:
  std::vector <Point2D<T>> pointsContainer;
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 6.0;
  Color pointColor = BLUE;
  Color color = RED;
  bool filled = false;

  Polygon2D() {}

  virtual ~Polygon2D() {}

  inline void addPoint(T xp, T yp)
  {
    this->pointsContainer.emplace_back(xp, yp);
    this->points++;
  }

  inline void checkClick(GLint xMouse, GLint yMouse, int sens)
  {
    Point2D<T> mousePos(static_cast<T>(xMouse), static_cast<T>(yMouse));
    for(auto& point : pointsContainer)
      point.checkClick(mousePos, sens);
  }

  inline void checkClick(const Point2D<GLint>& mousePos, int sens)
  {
    for(const auto& point : pointsContainer)
      point.checkClick(mousePos, sens);
  }

  inline void handleClick(GLint xMouse, GLint yMouse) 
  {
    for(auto& point : pointsContainer)
    {
      if(point.clicked)
      {
        point.setXY(xMouse, yMouse);
        break;
      }
    }
  }

  inline void release()
  {
    for(auto& point : pointsContainer)
    {
      point.release();
    }
  }

  inline void handleClick(const Point2D<GLint>& mousePos)
  {
    for(auto& point : pointsContainer)
    {
      if(point.clicked)
      {
        point.setXY(mousePos);
        break;
      }
    }
  }

  void draw() const
  {
    glLineWidth(lineWidth);
    color.setGLColor();

    if(filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for(const auto& point : pointsContainer)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

}; // end class Polygon2D

} // end namespace Utils