#pragma once

#include <GL/freeglut.h>
#include <vector>
#include "Point2D.h"
#include "Color.h"

namespace Utils
{

template <typename T>
class Rectangle
{
public:
  std::vector <Point2D<T>> pointsContainer;
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 6.0;
  Color pointColor = RED;
  Color color = BLACK;
  bool filled = false;
  bool clicked = false;

  Rectangle(const Point2D<T>& topleft, const Point2D<T>& topright,
            const Point2D<T>& bottomright, const Point2D<T>& bottomleft)
  {
    this->pointsContainer.emplace_back(topright);
    this->pointsContainer.emplace_back(topleft);
    this->pointsContainer.emplace_back(bottomleft);
    this->pointsContainer.emplace_back(bottomright);
  }

  Rectangle(const Point2D<T>& bottomleft, const Point2D<T>& topright)
  {
    this->pointsContainer.emplace_back(topright);
    this->pointsContainer.emplace_back(bottomleft.x(), topright.y());
    this->pointsContainer.emplace_back(bottomleft);
    this->pointsContainer.emplace_back(topright.x(), bottomleft.y());
  }

  Rectangle(T x1, T y1, T x2, T y2)
  {
    this->pointsContainer.emplace_back(x2, y2);
    this->pointsContainer.emplace_back(x1, y2);
    this->pointsContainer.emplace_back(x1, y1);
    this->pointsContainer.emplace_back(x2, y1);
  }

  virtual ~Rectangle() {}

  inline T width() const
  {
    return this->pointsContainer[0].x() - this->pointsContainer[2].x();
  }

  inline T height() const
  {
    return this->pointsContainer[0].y() - this->pointsContainer[2].y();
  }

  inline T left() const
  {
    return this->pointsContainer[2].x();
  }

  inline T right() const
  {
    return this->pointsContainer[0].x();
  }

  inline T top() const
  {
    return this->pointsContainer[0].y();
  }

  inline T bottom() const
  {
    return this->pointsContainer[3].y();
  }

  void draw() const
  {
    this->color.setGLColor();

    if (filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for (const auto& point : this->pointsContainer)
      glVertex2<T>(point);

    glEnd();
  }

  void drawPoints() const
  {
    this->pointColor.setGLColor();
    glPointSize(this->pointSize);

    glBegin(GL_POINTS);

    for (const auto& point : this->pointsContainer)
      glVertex2<T>(point);

    glEnd();
  }

}; // end class Rectangle

} // end namespace Utils
