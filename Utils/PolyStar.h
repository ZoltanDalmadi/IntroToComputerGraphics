#pragma once

#include "Circle.h"

namespace Utils
{

template <typename T>
class PolyStar
{
private:
  Circle<T> outer;
  Circle<T> inner;

public:
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 6.0;
  Color color = BLACK;
  Color pointColor = BLUE;
  bool filled = false;

  PolyStar(T cx, T cy, size_t spikes, T innerRadius, T outerRadius)
    : outer(cx, cy, outerRadius, spikes), inner(cx, cy, innerRadius, spikes)
  {
    double alpha = 2 * PI / (spikes * 2);
    inner.rotate(alpha);
  }

  /// Returns center point's X coordinate.
  inline T cx() const
  {
    return inner.cx();
  }

  /// Returns center point's Y coordinate.
  inline T cy() const
  {
    return inner.cy();
  }

  /// Returns center point.
  inline Point2D<T> c() const
  {
    return inner.c();
  }

  /// Returns a reference to center point.
  inline Point2D<T>& rc()
  {
    return inner.rc();
  }

  /// Returns inner radius.
  inline T getInnerRadius() const
  {
    return inner.getRadius();
  };

  /// Returns outer radius.
  inline T getOuterRadius() const
  {
    return outer.getRadius();
  };

  /// Returns number of spikes.
  inline size_t getSpikes() const
  {
    return inner.getPoints();
  }

  /// Sets number of spikes.
  inline void setSpikes(size_t p)
  {
    inner.setPoints(p);
    outer.setPoints(p);
  }

  /// Translate PolyStar with a given point.
  inline void translate(const Point2D<T>& point)
  {
    inner.translate(point);
    outer.translate(point);
  }

  /// Translate PolyStar with raw coordinates.
  inline void translate(T adx, T ady)
  {
    this->translate(Point2D<T>(adx, ady));
  }

  /// Translate PolyStar with a given Vector2D.
  inline void translate(const Vector2D<T>& vector)
  {
    inner.translate(vector);
    outer.translate(vector);
  }

  /// Increase PolyStar's spikes with postfix increment operator.
  inline void operator++(int)
  {
    inner++;
    outer++;
  }

  /// Decrease PolyStar's spikes with postfix decrement operator.
  inline void operator--(int)
  {
    inner--;
    outer--;
  }

  /// Increase PolyStar's spikes with prefix increment operator.
  inline void operator++()
  {
    ++inner;
    ++outer;
  }

  /// Decrease PolyStar's spikes with prefix decrement operator.
  inline void operator--()
  {
    --inner;
    --outer;
  }

  /// Transform PolyStar with a transformation matrix.
  void transform(const Matrix<T>& transform)
  {
    inner.transform(transform);
    outer.transform(transform);
  }

  /// Draw PolyStar with OpenGL calls.
  void draw() const
  {
    glLineWidth(lineWidth);
    color.setGLColor();

    size_t spikes = this->getSpikes();

    if(filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for(size_t i = 0; i < spikes; i++)
    {
      glVertex2<T>(outer.pointsContainer[i]);
      glVertex2<T>(inner.pointsContainer[i]);
    }

    glEnd();
  }

  /// Draw PolyStar's polygon points.
  void drawPoints() const
  {
    glPointSize(pointSize);
    pointColor.setGLColor();

    size_t spikes = this->getSpikes();

    glBegin(GL_POINTS);

    for(size_t i = 0; i < spikes; i++)
    {
      glVertex2<T>(outer.pointsContainer[i]);
      glVertex2<T>(inner.pointsContainer[i]);
    }

    glEnd();
  }

}; // end class PolyStar

} // end namespace Utils