#pragma once

#include <GL/freeglut.h>
#include "Point2D.h"
#include "Line.h"
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
  Color pointColor = RED;
  Color color = BLACK;
  bool filled = false;
  bool clicked = false;

  Polygon2D() {}

  virtual ~Polygon2D() {}

  inline void addPoint(T xp, T yp)
  {
    this->pointsContainer.emplace_back(xp, yp);
    this->points++;
  }

  inline void addPoint(const Point2D<T>& p)
  {
    this->pointsContainer.emplace_back(p);
    this->points++;
  }

  inline Point2D<T>* checkClick(GLint xMouse, GLint yMouse, int sens)
  {
    Point2D<T> *active = nullptr;
    Point2D<T> mousePos(static_cast<T>(xMouse), static_cast<T>(yMouse));

    for(auto& point : this->pointsContainer)
    {
      if(active = point.checkClick(mousePos, sens))
      {
        this->clicked = true;
        break;
      }
    }
 
    return active;
  }

  inline Point2D<T>* checkClick(const Point2D<GLint>& mousePos, int sens)
  {
    Point2D<T> *active = nullptr;

    for(auto& point : this->pointsContainer)
    {
      if(active = point.checkClick(mousePos, sens))
      {
        this->clicked = true;
        break;
      }
    }

    return active;
  }

  inline void release()
  {
    for(auto& point : this->pointsContainer)
      point.release();

    this->clicked = false;
  }

  inline void handleClick(GLint xMouse, GLint yMouse,
                          Point2D<T>* activePoint) 
  {
    if(this->clicked)
      activePoint->setXY(xMouse, yMouse);
  }

  inline void handleClick(const Point2D<GLint>& mousePos,
                          Point2D<T>* activePoint)
  {
    if(this->clicked)
      activePoint->setXY(mousePos);
  }

  inline void handleRightClick(GLint xMouse, GLint yMouse,
                               Point2D<T>* activePoint) 
  {
    if(this->clicked)
    {
      for(auto& point : pointsContainer)
      {
        if(activePoint == &point)
          continue;

        auto dx = point.x() - activePoint->x();
        auto dy = point.y() - activePoint->y();
        point.setXY(xMouse + dx, yMouse + dy);
      }

      activePoint->setXY(xMouse, yMouse);
    }
  }

  inline void handleRightClick(const Point2D<GLint>& mousePos,
                               Point2D<T>* activePoint)
  {
    this->handleRightClick(mousePos.x(), mousePos.y());
  }

  Polygon2D<T> clipWith(const Polygon2D<T>& clipper)
  {
    Polygon2D<T> output(*this);
    auto n = clipper.pointsContainer.size();

    for(size_t i = 0; i < n; i++)
    {
      if(output.pointsContainer.size())
      {
        auto other = i + 1;
        if(other == n)
          other = 0;

        Line<T> clipEdge(clipper.pointsContainer[i], clipper.pointsContainer[other]);


        Point2D<T> S = output.pointsContainer.back();
        Polygon2D<T> temp;

        for(const auto& point : output.pointsContainer)
        {
          if(clipEdge.isPointLeft(point))
          {
            if(!clipEdge.isPointLeft(S))
            {
              temp.addPoint(Line<T>::computeIntersectionPoint(
                Line<T>::getParams(S, point), clipEdge.getParams()));
            }
            temp.addPoint(point);
          }
          else if(clipEdge.isPointLeft(S))
          {
            temp.addPoint(Line<T>::computeIntersectionPoint(
              Line<T>::getParams(S, point), clipEdge.getParams()));
          }
          S = point;
        }

        output = temp;
      }
    }

    return output;
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

  void drawPoints() const
  {
    glPointSize(pointSize);
    pointColor.setGLColor();

    glBegin(GL_POINTS);

    for(const auto& point : pointsContainer)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

}; // end class Polygon2D

} // end namespace Utils