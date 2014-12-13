#pragma once
#include <vector>
#include <array>
#include <utility>
#include <map>
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
  typedef std::array<point_t *, 4> face_t;
  typedef std::pair<point_t *, point_t *> edge_t;
  typedef std::vector<std::vector<point_t>> points_t;
  typedef std::vector<std::vector<face_t *>> faces_t;
  typedef std::vector<std::vector<edge_t *>> edges_t;
  typedef std::map<face_t *, vector3D_t> normals_t;

  point_t center;
  size_t segments;
  T radius;
  void recalcPoints()
  {
    // empty container
    this->points.clear();

    // calculate points
    double step = Utils::PI / segments;

    points.emplace_back();
    points.back().emplace_back(center.x(), center.y(), radius);

    for (double phi = step; phi < Utils::PI; phi += step)
    {
      points.emplace_back();

      for (double theta = 0; theta < 2 * Utils::PI; theta += step)
      {
        points.back().emplace_back(
          center.x() + radius * std::cos(theta) * std::sin(phi),
          center.y() + radius * std::sin(theta) * std::sin(phi),
          center.z() + radius * std::cos(phi)
        );
      }

      points.back().shrink_to_fit();
    }

    points.emplace_back();
    points.back().emplace_back(center.x(), center.y(), -radius);

    // assign faces

    // assign edges

    // calculate normals
  }

public:
  points_t points;
  faces_t faces;
  edges_t edges;
  normals_t normals;
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
