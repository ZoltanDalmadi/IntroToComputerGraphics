#pragma once

#include <GL/freeglut.h>
#include <vector>
#include "Point2D.h"
#include "Point3D.h"
#include "Color.h"

namespace Utils
{

template <typename T>
class Cube
{
public:
  std::vector<Point3DH<T>> pointsContainer;
  std::vector<std::vector<Point3DH<T>*>> faces;
  GLfloat lineWidth = 2.0;
  GLfloat pointSize = 8.0;
  Color pointColor = RED;
  Color color = BLACK;

  // unit cube at origin
  Cube()
  {
    this->pointsContainer.emplace_back(0.5, 0.5, 0.5, 1);
    this->pointsContainer.emplace_back(-0.5, 0.5, 0.5, 1);
    this->pointsContainer.emplace_back(-0.5, -0.5, 0.5, 1);
    this->pointsContainer.emplace_back(0.5, -0.5, 0.5, 1);
    this->pointsContainer.emplace_back(0.5, 0.5, -0.5, 1);
    this->pointsContainer.emplace_back(-0.5, 0.5, -0.5, 1);
    this->pointsContainer.emplace_back(-0.5, -0.5, -0.5, 1);
    this->pointsContainer.emplace_back(0.5, -0.5, -0.5, 1);

    this->faces.emplace_back(std::initializer_list<Point3DH<T>*>
    {
      &pointsContainer[0], &pointsContainer[1],
      &pointsContainer[2], &pointsContainer[3]
    });

    this->faces.emplace_back(std::initializer_list<Point3DH<T>*>
    {
      &pointsContainer[4], &pointsContainer[0],
      &pointsContainer[3], &pointsContainer[7]
    });

    this->faces.emplace_back(std::initializer_list<Point3DH<T>*>
    {
      &pointsContainer[5], &pointsContainer[4],
      &pointsContainer[7], &pointsContainer[6]
    });

    this->faces.emplace_back(std::initializer_list<Point3DH<T>*>
    {
      &pointsContainer[1], &pointsContainer[5],
      &pointsContainer[6], &pointsContainer[2]
    });

    this->faces.emplace_back(std::initializer_list<Point3DH<T>*>
    {
      &pointsContainer[5], &pointsContainer[1],
      &pointsContainer[0], &pointsContainer[4]
    });

    this->faces.emplace_back(std::initializer_list<Point3DH<T>*>
    {
      &pointsContainer[7], &pointsContainer[3],
      &pointsContainer[2], &pointsContainer[6]
    });
  }

  void draw(const Matrix<T>& proj) const
  {
    this->color.setGLColor();
    glLineWidth(this->lineWidth);

    for (const auto& face : this->faces)
    {
      glBegin(GL_LINE_STRIP);

      for (const auto& point : face)
        glVertex2<T>(point->transformed(proj).normalized2D());

      glEnd();
    }
  }

  void drawPoints(const Matrix<T>& proj) const
  {
    this->pointColor.setGLColor();
    glPointSize(this->pointSize);

    for (const auto& face : this->faces)
    {
      glBegin(GL_POINTS);

      for (const auto& point : face)
        glVertex2<T>(point->transformed(proj).normalized2D());

      glEnd();
    }
  }

  virtual ~Cube()
  {
  }

}; // end class Cube

} // end namespace Utils
