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
  typedef Point3D<T> point3D_t;
  typedef Point2D<T> point2D_t;
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
    //Matrix<T> m(4, 4);
    //m.setToIdentity();
    //transformedPoints.clear();
    //transformedPoints.resize(points.size());

    //for (size_t i = 0; i < transformedPoints.size(); ++i)
    //{
    //  transformedPoints[i].resize(points[i].size());
    //}

    //transformPoints(m);

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

      // insert face into face container
      faces.emplace_back(face);
    }

    // assign edges -----------------------------------------------------------
  }

public:
  std::vector<std::vector<point_t>> points;
  std::vector<std::vector<point3D_t>> transformedPoints;
  std::vector<Face> faces;
  std::vector<Edge> edges;
  GLfloat lineWidth = 2.0;
  GLfloat pointSize = 8.0;
  Color pointColor = RED;
  Color edgeColor = LIGHT_GRAY;
  Color color = YELLOW;

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

  void transformPoints(const Matrix<T>& m)
  {
    for (size_t i = 0; i < points.size(); ++i)
      for (size_t j = 0, j < points[i].size(); ++j)
        transformedPoints[i][j] = points[i][j].transformed(m);
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

  void drawFaces(const Matrix<T>& proj, const Matrix<T>& rot,
                 const point_t& projCenter, const point_t& lightSource)
  {
    //this->transformPoints(rot);

    std::vector<Face> facesToDraw;

    for (auto& face : this->faces)
    {
      auto normal = face.normal.transformed(rot);
      auto centroid = face.centroid.transformed(rot);

      //vector3D_t u(face.vertices[0]->transformed(rot),
      //             face.vertices[1]->transformed(rot));

      //vector3D_t v(face.vertices[0]->transformed(rot),
      //             face.vertices[2]->transformed(rot));

      //vector3D_t u(*face.vertices[0], *face.vertices[1]);

      //vector3D_t v(*face.vertices[0], *face.vertices[2]);

      //auto normal = vector3D_t::crossProduct(u, v);
      //normal.normalize();

      // backface culling
      vector3D_t s(centroid, projCenter);
      s.normalize();

      if (vector3D_t::dotProduct(s, normal) > 0)
        facesToDraw.push_back(face);
    }

    std::sort(facesToDraw.begin(), facesToDraw.end(),
              [&rot](const Face & a, const Face & b)
    {
      return a.centroid.transformed(rot).z() < b.centroid.transformed(rot).z();
    });

    for (auto& face : facesToDraw)
    {
      auto normal = face.normal.transformed(rot);
      auto centroid = face.centroid.transformed(rot);
      vector3D_t f(centroid, lightSource);
      f.normalize();

      auto dp = static_cast<GLfloat>((vector3D_t::dotProduct(f, normal) + 1) / 2);

      auto Tm = proj * rot;

      glColor3f(dp, dp, dp);

      glBegin(GL_POLYGON);

      for (const auto& vertex : face.vertices)
        glVertex2<T>(vertex->transformed(Tm).normalized2D());

      glEnd();

      point_t endpoint(
        centroid.x() + normal.x() * 0.25,
        centroid.y() + normal.y() * 0.25,
        centroid.z() + normal.z() * 0.25
      );

      pointColor.setGLColor();
      glBegin(GL_LINES);
      glVertex2<T>(centroid.transformed(proj).normalized2D());
      glVertex2<T>(endpoint.transformed(proj).normalized2D());
      glEnd();

      //glColor3f(static_cast<GLfloat>(dp * 0.5),
      //          static_cast<GLfloat>(dp * 0.5),
      //          static_cast<GLfloat>(dp * 0.5));

      //glBegin(GL_LINE_LOOP);

      //for (const auto& vertex : face.vertices)
      //  glVertex2<T>(vertex->transformed(Tm).normalized2D());

      //glEnd();

      //glPointSize(this->pointSize);
      //this->pointColor.setGLColor();
      //glBegin(GL_POINTS);

      //for (const auto& vertex : face.vertices)
      //  glVertex2<T>(vertex->transformed(Tm).normalized2D());

      //glEnd();
    }

  }

  void drawEdges(const Matrix<T>& proj)
  {
    this->edgeColor.setGLColor();
    glLineWidth(this->lineWidth);

    this->transformPoints(proj);

    // horizontal lines
    for (size_t row = 1; row < segments; row++)
    {
      glBegin(GL_LINE_LOOP);

      for (size_t col = 0; col < segments * 2; col++)
        glVertex2<T>(transformedPoints[row][col]);

      glEnd();
    }

    // vertical lines
    for (size_t col = 0; col < segments * 2; col++)
    {
      glBegin(GL_LINE_STRIP);
      glVertex2<T>(transformedPoints.front().back());

      for (size_t row = 1; row < segments; row++)
        glVertex2<T>(transformedPoints[row][col]);

      glVertex2<T>(transformedPoints.back().back());

      glEnd();
    }
  }

}; // end class Sphere

} // end namespace Utils
