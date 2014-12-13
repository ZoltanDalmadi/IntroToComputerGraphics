#pragma once
#include <vector>
#include <array>
#include "Point3D.h"
#include "Vector3D.h"

namespace Utils
{

template <typename T>
class Sphere
{
private:
  typedef Point3DH<T> point_t;
  typedef Vector3D<T> vector3D_t;

  struct Face
  {
    std::vector<point_t *> vertices;
    vector3D_t normal;
    point_t centroid;
  };

  struct Edge
  {
    std::array<point_t *, 2> vertices;
  };

  point_t center;
  size_t segments;
  T radius;
  void recalcPoints()
  {
    // empty container
    this->points.clear();

    // calculate points -------------------------------------------------------
    double step = Utils::PI / segments;

    // add top Z point
    points.emplace_back();
    points.back().emplace_back(center.x(), center.y(), radius);

    // add points in a circular fashion
    for (double phi = step; phi < Utils::PI; phi += step)
    {
      // start columns
      points.emplace_back();

      for (double theta = 0; theta < 2 * Utils::PI; theta += step)
      {
        // fill rows
        points.back().emplace_back(
          center.x() + radius * std::cos(theta) * std::sin(phi),
          center.y() + radius * std::sin(theta) * std::sin(phi),
          center.z() + radius * std::cos(phi)
        );
      }

      // reduce memory usage
      points.back().shrink_to_fit();
    }

    // add bottom Z point
    points.back().emplace_back(center.x(), center.y(), -radius);

    // assign faces -----------------------------------------------------------
    // add top triangles

    // assign edges -----------------------------------------------------------
    // calculate normals
  }

public:
  std::vector<std::vector<point_t>> points;
  std::vector<Face> faces;
  std::vector<Edge> edges;
  GLfloat lineWidth = 2.0;
  GLfloat pointSize = 8.0;
  Color pointColor = RED;
  Color edgeColor = BLACK;
  Color color = ORANGE;

  // unit radius sphere at origin
  Sphere(size_t segments = 8)
    : center(0, 0, 0), segments(segments), radius(1.0f)
  {
    this->recalcPoints();
  }

  virtual ~Sphere()
  {}

  inline size_t getSegments()
  {
    return this->segments;
  }

  /// Increase Spheres's segments with postfix increment operator.
  inline void operator++(int)
  {
    this->segments++;
    this->recalcPoints();
  }

  /// Decrease Sphere's segments with postfix decrement operator.
  inline void operator--(int)
  {
    this->segments--;
    this->recalcPoints();
  }

  /// Increase Sphere's segments with prefix increment operator.
  inline void operator++()
  {
    this->segments++;
    this->recalcPoints();
  }

  /// Decrease Sphere's segments with prefix decrement operator.
  inline void operator--()
  {
    this->segments--;
    this->recalcPoints();
  }

  void drawPoints(const Matrix<T>& proj) const
  {
    this->pointColor.setGLColor();
    glPointSize(this->pointSize);

    glBegin(GL_POINTS);

    for (const auto& line : this->points)
      for (const auto& point : line)
        glVertex2<T>(point.transformed(proj).normalized2D());

    glEnd();
  }

  void drawEdges(const Matrix<T>& proj) const
  {
    this->edgeColor.setGLColor();
    glLineWidth(this->lineWidth);

    // horizontal lines
    for (size_t row = 1; row < segments; row++)
    {
      glBegin(GL_LINE_LOOP);

      for (size_t col = 0; col < segments * 2; col++)
        glVertex2<T>(points[row][col].transformed(proj).normalized2D());

      glEnd();
    }

    // vertical lines
    for (size_t col = 0; col < segments * 2; col++)
    {
      glBegin(GL_LINE_STRIP);
      glVertex2<T>(points.front().back().transformed(proj).normalized2D());

      for (size_t row = 1; row < segments; row++)
        glVertex2<T>(points[row][col].transformed(proj).normalized2D());

      glVertex2<T>(points.back().back().transformed(proj).normalized2D());

      glEnd();
    }
  }

}; // end class Sphere

} // end namespace Utils
