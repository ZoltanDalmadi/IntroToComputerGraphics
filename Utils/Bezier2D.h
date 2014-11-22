#pragma once

#include <GL/freeglut.h>
#include <cmath>
#include "Point2D.h"
#include "Line.h"
#include "Color.h"

namespace Utils
{

size_t binomialCoeff(size_t n, size_t k)
{
  if (k == 0 || k == n)
    return 1;

  return binomialCoeff(n - 1, k - 1) + binomialCoeff(n - 1, k);
}

double bernsteinPolynomial(size_t n, size_t i, double t)
{
  return binomialCoeff(n, i) * std::pow(t, i) * std::pow(1 - t, n - i);
}

template <typename T>
class Bezier2D
{
protected:
  size_t points = 0;

public:
  std::vector <Point2D<T>> controlPoints;
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 6.0;
  Color pointColor = RED;
  Color curveColor = BLACK;
  bool clicked = false;

  Bezier2D() {}

  virtual ~Bezier2D()
  {}

  inline void addPoint(T xp, T yp)
  {
    this->controlPoints.emplace_back(xp, yp);
    this->points++;
  }

  inline void addPoint(const Point2D<T>& p)
  {
    this->controlPoints.emplace_back(p);
    this->points++;
  }

  inline size_t getPoints() const
  {
    return this->points;
  }

  inline Point2D<T> *checkClick(GLint xMouse, GLint yMouse, int sens)
  {
    Point2D<T> *active = nullptr;
    Point2D<T> mousePos(static_cast<T>(xMouse), static_cast<T>(yMouse));

    for (auto& point : this->controlPoints)
    {
      if ((active = point.checkClick(mousePos, sens)))
      {
        this->clicked = true;
        break;
      }
    }

    return active;
  }

  inline Point2D<T> *checkClick(const Point2D<GLint>& mousePos, int sens)
  {
    Point2D<T> *active = nullptr;

    for (auto& point : this->controlPoints)
    {
      if ((active = point.checkClick(mousePos, sens)))
      {
        this->clicked = true;
        break;
      }
    }

    return active;
  }

  inline void handleClick(GLint xMouse, GLint yMouse,
                          Point2D<T> *activePoint)
  {
    if (this->clicked)
      activePoint->setXY(xMouse, yMouse);
  }

  inline void handleClick(const Point2D<GLint>& mousePos,
                          Point2D<T> *activePoint)
  {
    if (this->clicked)
      activePoint->setXY(mousePos);
  }

  inline void release()
  {
    for (auto& point : this->controlPoints)
      point.release();

    this->clicked = false;
  }

  void drawBernstein() const
  {
    glLineWidth(lineWidth);
    curveColor.setGLColor();

    glBegin(GL_LINE_STRIP);

    size_t n = this->points;

    for (double t = 0.0f; t <= 1.0; t += 0.01)
    {
      double sumX = 0.0f;
      double sumY = 0.0f;

      for (size_t i = 0; i < n; i++)
      {
        double bp = bernsteinPolynomial(n - 1, i, t);
        sumX += bp * this->controlPoints[i].x();
        sumY += bp * this->controlPoints[i].y();
      }

      glVertex2<T>(sumX, sumY);
    }

    glVertex2<T>(this->controlPoints.back());

    glEnd();
  }

  void drawPoints() const
  {
    glPointSize(pointSize);
    pointColor.setGLColor();

    glBegin(GL_POINTS);

    for (const auto& point : controlPoints)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

  void drawControlPolygon() const
  {
    glBegin(GL_LINE_STRIP);

    for (const auto& point : controlPoints)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

}; // end class Bezier2D

} // end namespace Utils
