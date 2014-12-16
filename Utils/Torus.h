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
    double step = Utils::PI / this->segments;

    // add points in a circular fashion
    for (double theta = 0.0f; theta < 2 * Utils::PI; theta += step)
    {
      // start columns
      this->points.emplace_back();

      for (double phi = 0.0f; phi < 2 * Utils::PI; phi += step)
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

    // add middle quads
//    auto segment2 = this->segments * 2;
//    for (size_t i = 2; i < this->segments; ++i)
//    {
//      for (size_t j = 0; j < segment2; ++j)
//      {
//        // create face
//        Face face;
//        face.vertices.emplace_back(&this->points[i - 1][j]);
//        face.vertices.emplace_back(&this->points[i][j]);

//        if (j < segment2 - 1)
//        {
//          face.vertices.emplace_back(&this->points[i][j + 1]);
//          face.vertices.emplace_back(&this->points[i - 1][j + 1]);
//        }
//        else
//        {
//          face.vertices.emplace_back(&this->points[i][0]);
//          face.vertices.emplace_back(&this->points[i - 1][0]);
//        }

//        this->prepareFace(face);

//        // insert face into face container
//        this->faces.emplace_back(face);
//      }
//    }
  }

public:

  // unit major radius torus at origin
  Torus(size_t segments = 64)
    : Mesh<T>(segments), R(1.0f), r(0.4f)
  {
    this->recalcPoints();
  }

  virtual ~Torus()
  {}

}; // end class Sphere

} // end namespace Utils
