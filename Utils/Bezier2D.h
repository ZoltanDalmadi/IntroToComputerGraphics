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
  std::vector <Color> colorCycle;
  GLfloat lineWidth = 2.0;
  GLfloat interpolationLinesWidth = 1.0;
  GLfloat pointSize = 8.0;
  Color pointColor = RED;
  Color curveColor = BLACK;
  Color controlPolygonColor = VERY_LIGHT_GRAY;
  bool clicked = false;

  Bezier2D()
  {
    this->colorCycle.emplace_back(GREEN);
    this->colorCycle.emplace_back(BLUE);
    this->colorCycle.emplace_back(MAGENTA);
  }

  virtual ~Bezier2D()
  {}

  inline void addPoint(T xp, T yp)
  {
    this->controlPoints.emplace_back(xp, yp);
    this->controlPoints.back().color = pointColor;
    this->points++;
  }

  inline void addPoint(const Point2D<T>& p)
  {
    this->controlPoints.emplace_back(p);
    this->controlPoints.back().color = pointColor;
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
    if (!this->points)
      return;

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

  Point2D<T> calcPointOnCurve(double t) const
  {
    auto n = this->controlPoints.size();
    auto temp = this->controlPoints;

    for (size_t r = 1; r < n; r++)
      for (size_t i = 0; i < n - r; i++)
        temp[i] = (1 - t) * temp[i] + t * temp[i + 1];

    return temp[0];
  }

  void draw() const
  {
    if (!this->points)
      return;

    glLineWidth(lineWidth);
    this->curveColor.setGLColor();

    glBegin(GL_LINE_STRIP);

    for (double t = 0.0f; t <= 1.0; t += 0.01)
    {
      glVertex2<T>(calcPointOnCurve(t));
    }

    glVertex2<T>(this->controlPoints.back());

    glEnd();
  }

  void drawUntilParam(double param) const
  {
    if (!this->points)
      return;

    glLineWidth(lineWidth);
    this->curveColor.setGLColor();

    glBegin(GL_LINE_STRIP);

    for (double t = 0.0f; t <= param; t += 0.01)
    {
      glVertex2<T>(calcPointOnCurve(t));
    }

    glVertex2<T>(calcPointOnCurve(param));

    glEnd();
  }

  void drawPoints() const
  {
    if (!this->points)
      return;

    glPointSize(pointSize);

    glBegin(GL_POINTS);

    for (const auto& point : this->controlPoints)
    {
      point.color.setGLColor();
      glVertex2<T>(point);
    }

    glEnd();
  }

  void drawControlPolygon() const
  {
    if (!this->points)
      return;

    this->controlPolygonColor.setGLColor();
    glBegin(GL_LINE_STRIP);

    for (const auto& point : this->controlPoints)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

  void drawInterPolations(double t, bool points = true) const
  {
    if (!this->points)
      return;

    glLineWidth(interpolationLinesWidth);

    auto n = this->controlPoints.size();
    auto temp = this->controlPoints;

    auto colorIterator = colorCycle.begin();

    for (size_t r = 1; r < n; r++)
    {
      for (size_t i = 0; i < n - r; i++)
      {
        temp[i] = (1 - t) * temp[i] + t * temp[i + 1];
      }

      if (colorIterator == colorCycle.end())
        colorIterator = colorCycle.begin();

      colorIterator->setGLColor(); 

      glBegin(GL_LINE_STRIP);

      for (size_t j = 0; j < n - r; j++)
      {
        glVertex2<T>(temp[j]);
      }

      glEnd();

      if (points)
      {
        glPointSize(pointSize - 2);
        glBegin(GL_POINTS);

        for (size_t j = 0; j < n - r; j++)
        {
          glVertex2<T>(temp[j]);
        }

        glEnd();
      }

      colorIterator++;
    }
    this->curveColor.setGLColor();
    glBegin(GL_POINTS);
    glVertex2<T>(temp[0]);
    glEnd();
  }


}; // end class Bezier2D

} // end namespace Utils
