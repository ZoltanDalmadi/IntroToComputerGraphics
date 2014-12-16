#pragma once
#include "Mesh.h"

namespace Utils
{

template <typename T>
class Sphere : public Mesh<T>
{
private:
  typedef typename Mesh<T>::point_t point_t;
  typedef typename Mesh<T>::point3D_t point3D_t;
  typedef typename Mesh<T>::point2D_t point2D_t;
  typedef typename Mesh<T>::vector3D_t vector3D_t;

  typedef typename Mesh<T>::Face Face;

  T radius;

  virtual void recalcPoints()
  {
    auto segment2 = this->segments * 2;

    // empty container
    this->points.clear();

    // calculate points -------------------------------------------------------
    double step = Utils::PI / this->segments;

    // add top Z point
    this->points.emplace_back();
    this->points.back().emplace_back(
      this->center.x(), this->center.y(), this->radius
    );

    // add points in a circular fashion
    for (double phi = step; phi < Utils::PI; phi += step)
    {
      // start columns
      this->points.emplace_back();

      for (double theta = 0; theta < 2 * Utils::PI; theta += step)
      {
        // fill rows
        this->points.back().emplace_back(
          this->center.x() + this->radius * std::cos(theta) * std::sin(phi),
          this->center.y() + this->radius * std::sin(theta) * std::sin(phi),
          this->center.z() + this->radius * std::cos(phi)
        );
      }
    }

    // add bottom Z point
    this->points.emplace_back();
    this->points.back().emplace_back(
      this->center.x(), this->center.y(), -this->radius
    );

    // assign faces -----------------------------------------------------------
    this->faces.clear();
    point_t& topPoint = this->points.front().back();
    point_t& bottomPoint = this->points.back().back();

    // add top triangles
    for (size_t i = 0; i < segment2; ++i)
    {
      // create face
      Face face;
      face.vertices.emplace_back(&topPoint);
      face.vertices.emplace_back(&this->points[1][i]);

      if (i < segment2 - 1)
        face.vertices.emplace_back(&this->points[1][i + 1]);
      else
        face.vertices.emplace_back(&this->points[1][0]);

      this->prepareFace(face);

      // insert face into face container
      this->faces.emplace_back(face);
    }

    // add middle quads
    for (size_t i = 2; i < this->segments; ++i)
    {
      for (size_t j = 0; j < segment2; ++j)
      {
        // create face
        Face face;
        face.vertices.emplace_back(&this->points[i - 1][j]);
        face.vertices.emplace_back(&this->points[i][j]);

        if (j < segment2 - 1)
        {
          face.vertices.emplace_back(&this->points[i][j + 1]);
          face.vertices.emplace_back(&this->points[i - 1][j + 1]);
        }
        else
        {
          face.vertices.emplace_back(&this->points[i][0]);
          face.vertices.emplace_back(&this->points[i - 1][0]);
        }

        this->prepareFace(face);

        // insert face into face container
        this->faces.emplace_back(face);
      }
    }

    // add bottom triangles
    for (size_t i = 0; i < segment2; ++i)
    {
      // create face
      Face face;
      face.vertices.emplace_back(&this->points[this->segments - 1][i]);
      face.vertices.emplace_back(&bottomPoint);

      if (i < segment2 - 1)
        face.vertices.emplace_back(&this->points[this->segments - 1][i + 1]);
      else
        face.vertices.emplace_back(&this->points[this->segments - 1][0]);

      this->prepareFace(face);

      // insert face into face container
      this->faces.emplace_back(face);
    }
  }

public:

  // unit radius sphere at origin
  Sphere(size_t segments = 16, std::string label = "Sphere")
    : Mesh<T>(segments, label), radius(1.0f)
  {
    this->recalcPoints();
  }

  virtual ~Sphere()
  {}

}; // end class Sphere

} // end namespace Utils
