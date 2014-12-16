#pragma once
#include "Mesh.h"

namespace Utils
{

template <typename T>
class Torus : public Mesh<T>
{
private:
  typedef typename Mesh<T>::point_t point_t;
  typedef typename Mesh<T>::point3D_t point3D_t;
  typedef typename Mesh<T>::point2D_t point2D_t;
  typedef typename Mesh<T>::vector3D_t vector3D_t;

  typedef typename Mesh<T>::Face Face;

  T R;
  T r;

  virtual void recalcPoints()
  {
    // empty container
    this->points.clear();

    // calculate points -------------------------------------------------------
    double step = 2 * Utils::PI / this->segments;

    // add points in a circular fashion
    for (double phi = 0.0f; phi < 2 * Utils::PI; phi += step)
    {
      // start columns
      this->points.emplace_back();

      for (double theta = 0.0f; theta < 2 * Utils::PI; theta += step)
      {
        // fill rows
        this->points.back().emplace_back(
          this->center.x() + (this->R + this->r * std::cos(theta)) * std::cos(phi),
          this->center.y() + (this->R + this->r * std::cos(theta)) * std::sin(phi),
          this->center.z() + this->r * std::sin(theta)
        );
      }
    }

    // assign faces -----------------------------------------------------------
    this->faces.clear();

    // add quads
    for (size_t i = 0; i < this->segments; ++i)
    {
      for (size_t j = 0; j < this->segments; ++j)
      {
        // create face
        Face face;
        face.vertices.resize(4);

        face.vertices[0] = &this->points[i][j];
        face.vertices[1] = &this->points[i + 1][j];
        face.vertices[2] = &this->points[i + 1][j + 1];
        face.vertices[3] = &this->points[i][j + 1];

        if (j == this->segments - 1)
        {
          face.vertices[2] = &this->points[i + 1][0];
          face.vertices[3] = &this->points[i][0];
        }

        if (i == this->segments - 1)
        {
          face.vertices[1] = &this->points[0][j];
          face.vertices[2] = &this->points[0][j+1];

          if (j == this->segments - 1)
          {
            face.vertices[2] = &this->points[0][0];
          }
        }

        this->prepareFace(face);

        // insert face into face container
        this->faces.emplace_back(face);
      }
    }
  }

public:

  // unit major radius torus at origin
  Torus(size_t segments = 16, std::string label = "Torus")
    : Mesh<T>(segments, label), R(1.0f), r(0.4f)
  {
    this->recalcPoints();
  }

  virtual ~Torus()
  {}

}; // end class Sphere

} // end namespace Utils
