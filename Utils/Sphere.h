#pragma once
#include <vector>
#include <algorithm>
#include "Point3D.h"
#include "Vector3D.h"

namespace Utils
{

template <typename T>
class Sphere
{
private:
  typedef Point3DH<T> point_t;
  typedef Point3D<T> point3D_t;
  typedef Point2D<T> point2D_t;
  typedef Vector3D<T> vector3D_t;

  struct Face
  {
    std::vector<point_t *> vertices;
    vector3D_t normal;
    point_t centroid;
  };

  point_t center;
  size_t segments;
  T radius;

  void prepareFace(Face& face)
  {
    // calculate normal
    vector3D_t u(*face.vertices[0], *face.vertices[1]);
    vector3D_t v(*face.vertices[0], *face.vertices[2]);
    face.normal = vector3D_t::crossProduct(u, v);
    face.normal.normalize();

    // calculate centroid
    T x = 0, y = 0, z = 0;
    auto numOfVertices = face.vertices.size();

    for (const auto& vertex : face.vertices)
    {
      x += vertex->x();
      y += vertex->y();
      z += vertex->z();
    }

    x /= numOfVertices;
    y /= numOfVertices;
    z /= numOfVertices;

    face.centroid = std::move(point_t(x, y, z, 1));
  }

  void recalcPoints()
  {
    auto segment2 = segments * 2;

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
    }

    // add bottom Z point
    points.emplace_back();
    points.back().emplace_back(center.x(), center.y(), -radius);

    // assign faces -----------------------------------------------------------
    this->faces.clear();
    point_t& topPoint = points.front().back();
    point_t& bottomPoint = points.back().back();

    // add top triangles
    for (size_t i = 0; i < segment2; ++i)
    {
      // create face
      Face face;
      face.vertices.emplace_back(&topPoint);
      face.vertices.emplace_back(&points[1][i]);

      if (i < segment2 - 1)
        face.vertices.emplace_back(&points[1][i + 1]);
      else
        face.vertices.emplace_back(&points[1][0]);

      prepareFace(face);

      // insert face into face container
      faces.emplace_back(face);
    }

    // add middle quads
    for (size_t i = 2; i < segments; ++i)
    {
      for (size_t j = 0; j < segment2; ++j)
      {
        // create face
        Face face;
        face.vertices.emplace_back(&points[i - 1][j]);
        face.vertices.emplace_back(&points[i][j]);

        if (j < segment2 - 1)
        {
          face.vertices.emplace_back(&points[i][j + 1]);
          face.vertices.emplace_back(&points[i - 1][j + 1]);
        }
        else
        {
          face.vertices.emplace_back(&points[i][0]);
          face.vertices.emplace_back(&points[i - 1][0]);
        }

        prepareFace(face);

        // insert face into face container
        faces.emplace_back(face);
      }
    }

    // add bottom triangles
    for (size_t i = 0; i < segment2; ++i)
    {
      // create face
      Face face;
      face.vertices.emplace_back(&points[segments - 1][i]);
      face.vertices.emplace_back(&bottomPoint);

      if (i < segment2 - 1)
        face.vertices.emplace_back(&points[segments - 1][i + 1]);
      else
        face.vertices.emplace_back(&points[segments - 1][0]);

      prepareFace(face);

      // insert face into face container
      faces.emplace_back(face);
    }

    // assign edges -----------------------------------------------------------
  }

public:
  std::vector<std::vector<point_t>> points;
  std::vector<Face> faces;
  GLfloat lineWidth = 2.0;
  GLfloat pointSize = 8.0;
  Color pointColor = DARK_RED;
  Color edgeColor = LIGHT_GRAY;
  Color normalColor = DARK_YELLOW;
  Color color = YELLOW;
  bool filled = true;
  bool drawEdges = false;
  bool drawNormals = false;
  bool drawPoints = false;
  bool backfaceCull = true;

  // unit radius sphere at origin
  Sphere(size_t segments = 16)
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

  void drawFaces(const Matrix<T>& proj, const Matrix<T>& rot,
                 const point_t& projCenter, const point_t& lightSource)
  {
    std::vector<Face *> facesToDraw;

    if (backfaceCull)
    {
      for (auto& face : this->faces)
      {
        auto normal = face.normal.transformed(rot);
        auto centroid = face.centroid.transformed(rot);

        // backface culling
        vector3D_t s(centroid, projCenter);
        s.normalize();

        // add to visible faces
        if (vector3D_t::dotProduct(s, normal) > 0)
          facesToDraw.emplace_back(&face);
      }
    }
    else
    {
      for (auto& face : this->faces)
        facesToDraw.emplace_back(&face);
    }

    // order visible faces by their centroid's Z coordinate
    std::sort(facesToDraw.begin(), facesToDraw.end(),
              [&rot](const Face * a, const Face * b)
    {
      return a->centroid.transformed(rot).z() < b->centroid.transformed(rot).z();
    });

    // draw visible faces
    for (const auto& face : facesToDraw)
    {
      auto Tm = proj * rot;

      std::vector<point2D_t> transformedPoints;

      for (const auto& vertex : face->vertices)
        transformedPoints.emplace_back(vertex->transformed(Tm).normalized2D());

      auto normal = face->normal.transformed(rot);
      auto centroid = face->centroid.transformed(rot);

      if (filled)
      {
        vector3D_t f(centroid, lightSource);
        f.normalize();

        auto dp =
          static_cast<GLfloat>(
            (vector3D_t::dotProduct(f, normal) + 1) / 2
          );


        glColor3f(dp, dp, dp);

        glBegin(GL_POLYGON);

        for (const auto& vertex : transformedPoints)
          glVertex2<T>(vertex);

        glEnd();
      }

      if (drawEdges)
      {
        edgeColor.setGLColor();
        glLineWidth(lineWidth);

        glBegin(GL_LINE_STRIP);

        for (const auto& vertex : transformedPoints)
          glVertex2<T>(vertex);

        glEnd();

        // GL_LINE_LOOP is broken on linux, so we draw last line manually
        glBegin(GL_LINE_STRIP);
        glVertex2<T>(transformedPoints.back());
        glVertex2<T>(transformedPoints.front());
        glEnd();
      }

      if (drawNormals)
      {
        point_t endpoint(
          centroid.x() + normal.x() * 0.1,
          centroid.y() + normal.y() * 0.1,
          centroid.z() + normal.z() * 0.1
        );

        this->normalColor.setGLColor();
        glBegin(GL_LINES);
        glVertex2<T>(centroid.transformed(proj).normalized2D());
        glVertex2<T>(endpoint.transformed(proj).normalized2D());
        glEnd();
      }

      if (drawPoints)
      {
        glPointSize(this->pointSize);
        this->pointColor.setGLColor();
        glBegin(GL_POINTS);

        for (const auto& vertex : transformedPoints)
          glVertex2<T>(vertex);

        glEnd();
      }
    }
  }

}; // end class Sphere

} // end namespace Utils
