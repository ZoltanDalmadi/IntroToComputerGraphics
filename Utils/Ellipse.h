#pragma once

#include <GL/freeglut.h>
#include <vector>
#include "functions.h"
#include "Point2D.h"
#include "Vector2D.h"
#include "Color.h"
#include "Polygon2D.h"

namespace Utils
{

template <typename T>
class Ellipse
{
protected:
  Point2D<T> centre;
  T rx;
  T ry;
  size_t points = 48;

public:
  GLfloat lineWidth = 1.0;
  GLfloat pointSize = 6.0;
  GLfloat centrePointSize = 10.0;
  std::vector <Point2D<T>> pointsContainer;
  Color color = BLACK;
  Color pointColor = BLUE;
  Color centrePointColor = RED;
  bool filled = false;

  inline Ellipse(T cx, T cy, T rx, T ry) : centre(cx, cy), rx(rx), ry(ry)
  {
    this->recalcPoints();
  }

  inline Ellipse(T cx, T cy, T rx, T ry, size_t points)
    : centre(cx, cy), rx(rx), ry(ry), points(points)
  {
    this->recalcPoints();
  }

  inline Ellipse(Point2D<T> c, T rx, T ry) : centre(c), rx(rx), ry(ry)
  {
    this->recalcPoints();
  }

  virtual ~Ellipse() {}

  /// Returns center point's X coordinate.
  inline T cx() const
  {
    return centre.x();
  }

  /// Returns center point's Y coordinate.
  inline T cy() const
  {
    return centre.y();
  }

  /// Returns center point.
  inline Point2D<T> c() const
  {
    return centre;
  }

  /// Returns a pointer to center point.
  inline Point2D<T> *pc()
  {
    return centre.getPointer();
  }

  /// Returns a reference to center point.
  inline Point2D<T>& rc()
  {
    return centre;
  }

  /// Sets center point.
  inline void setCentre(const Point2D<T>& P)
  {
    this->setCentre(P.x(), P.y());
  }

  /// Sets center point. (overload)
  inline void setCentre(T x, T y)
  {
    centre.setX(x);
    centre.setY(y);
    recalcPoints();
  }

  /// Returns X radius.
  inline T getRadiusX() const
  {
    return rx;
  };

  /// Returns Y radius.
  inline T getRadiusY() const
  {
    return ry;
  };

  /// Sets X radius.
  inline void setRadiusX(T radius)
  {
    rx = radius;
    this->recalcPoints();
  };

  /// Sets Y radius.
  inline void setRadiusY(T radius)
  {
    ry = radius;
    this->recalcPoints();
  };

  /// Returns number of points.
  inline size_t getPoints() const
  {
    return points;
  }

  /// Sets number of points.
  inline void setPoints(size_t p)
  {
    points = p;
    this->recalcPoints();
  }

  /// Translate Ellipse with a given point.
  inline void translate(const Point2D<T>& point)
  {
    centre += point;
  }

  /// Translate Ellipse with raw coordinates.
  inline void translate(T adx, T ady)
  {
    this->translate(Point2D<T>(adx, ady));
  }

  /// Translate Ellipse with a given Vector2D.
  inline void translate(const Vector2D<T>& vector)
  {
    centre.translate(vector);

    for (auto& point : this->pointsContainer)
      point.translate(vector);
  }

  /// Increase Ellipse's points with postfix increment operator.
  inline void operator++(int)
  {
    this->points++;
    this->recalcPoints();
  }

  /// Decrease Ellipse's points with postfix decrement operator.
  inline void operator--(int)
  {
    this->points--;
    this->recalcPoints();
  }

  /// Increase Ellipse's points with prefix increment operator.
  inline void operator++()
  {
    this->points++;
    this->recalcPoints();
  }

  /// Decrease Ellipse's points with prefix decrement operator.
  inline void operator--()
  {
    this->points--;
    this->recalcPoints();
  }

  /// Recalculate points.
  virtual void recalcPoints()
  {
    this->pointsContainer.clear();
    double gap = 2 * PI / points;

    for (size_t i = 0; i < points; ++i)
    {
      pointsContainer.emplace_back(static_cast<T>(centre.x() + rx * cos(i * gap)),
                                   static_cast<T>(centre.y() + ry * sin(i * gap)));
    }
  }

  /// Transform Ellipse with a transformation matrix.
  void transform(const Matrix<T>& transform)
  {
    for (auto& point : pointsContainer)
    {
      point.transform(transform);
    }
  }

  void rotate(double alpha)
  {
    this->pointsContainer.clear();
    double gap = 2 * PI / points;

    for (size_t i = 0; i < points; ++i)
    {
      pointsContainer.emplace_back(static_cast<T>(centre.x() + rx * cos(
                                                    i * gap + alpha)),
                                   static_cast<T>(centre.y() + ry * sin(i * gap + alpha)));
    }
  }

  /// Draw Ellipse with OpenGL calls.
  void draw() const
  {
    glLineWidth(lineWidth);
    color.setGLColor();

    if (filled)
      glBegin(GL_POLYGON);
    else
      glBegin(GL_LINE_LOOP);

    for (const auto& point : pointsContainer)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

  /// Draw Ellipse's polygon points.
  void drawPoints() const
  {
    glPointSize(pointSize);
    pointColor.setGLColor();

    glBegin(GL_POINTS);

    for (const auto& point : pointsContainer)
    {
      glVertex2<T>(point);
    }

    glEnd();
  }

  /// Draw center point.
  void drawCenterPoint() const
  {
    glPointSize(pointSize);
    centrePointColor.setGLColor();
    glBegin(GL_POINTS);
    glVertex2<T>(centre);
    glEnd();
  }

  /// Draw all diagonals of Ellipse's polygon.
  void drawDiagonals() const
  {
    glLineWidth(lineWidth);
    color.setGLColor();

    glBegin(GL_LINES);

    for (size_t i = 0; i < points; i++)
    {
      for (size_t j = i + 1; j < points; j++)
      {
        glVertex2<T>(pointsContainer[i]);
        glVertex2<T>(pointsContainer[j]);
      }
    }

    glEnd();
  }

  /// Draw evolvents from this ellipse.
  /// n: number of evolvents
  /// m: draw until this point. Depends on value of member variable "points".
  void drawEvolvents(size_t n, double m) const
  {
    glLineWidth(lineWidth);
    color.setGLColor();

    double gap = 2 * PI / points;
    double gap2 = 2 * PI / n;
    T cx = this->cx();
    T cy = this->cy();
    double theta;
    double alpha;
    double at;

    for (size_t j = 0; j < n; j++)
    {
      alpha = j * gap2;

      glBegin(GL_LINE_STRIP);

      for (double i = 0; i <= m; i++)
      {
        theta = i * gap;
        at = alpha + theta;
        glVertex2<T>(Point2D<T>(cx + rx * (cos(at) + theta * sin(at)),
                                cy + ry * (sin(at) - theta * cos(at))));
      }

      glEnd();
    }
  }

  Polygon2D<T> toPolygon2D() const
  {
    Polygon2D<T> output;
    output.color = this->color;
    output.pointColor = this->pointColor;
    output.filled = this->filled;
    output.lineWidth = this->lineWidth;
    output.pointSize = this->pointSize;

    for (const auto& point : this->pointsContainer)
      output.addPoint(point);

    return output;
  }

}; // end class Ellipse

} // end namespace Utils
