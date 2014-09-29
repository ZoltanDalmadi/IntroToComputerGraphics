#pragma once

#include <GL/freeglut.h>
#include <cmath>
#include "Point2D.h"
#include "Color.h"

namespace Utils {

  template <typename T>
  class Line {
  private:
    // Endpoints.
    Point2D<T> pt1;
    Point2D<T> pt2;

  public:
    GLfloat lineWidth = 1.0;
    GLfloat pointSize = 10.0;
    Utils::Color color = Utils::BLACK;
    Utils::Color pointsColor = Utils::RED;
    bool smooth = true;
    bool smoothPoints = true;

    // Init Line with raw coordinates.
    Line(T a1, T a2, T b1, T b2) : pt1(a1, a2), pt2(b1, b2) {}

    // Init Line with existing points.
    Line(Point2D<T> a, Point2D<T> b) : pt1(a), pt2(b) {}

    // Returns starting point's X coordinate.
    inline T x1() const {
      return pt1.x();
    }

    // Returns ending point's X coordinate.
    inline T x2() const {
      return pt2.x();
    }

    // Returns starting point's Y coordinate.
    inline T y1() const {
      return pt1.y();
    }

    // Returns ending point's Y coordinate.
    inline T y2() const {
      return pt2.y();
    }

    // Returns starting point of Line.
    inline Point2D<T> p1() const {
      return pt1;
    }

    // Returns ending point of Line.
    inline Point2D<T> p2() const {
      return pt2;
    }

    // Returns a pointer to starting point of Line.
    inline Point2D<T>* pp1() {
      return pt1.getPointer();
    }

    // Returns a pointer to ending point of Line.
    inline Point2D<T>* pp2() {
      return pt2.getPointer();
    }

    // Returns a reference to starting point of Line.
    inline Point2D<T>& rp1() {
      return pt1;
    }

    // Returns a reference to ending point of Line.
    inline Point2D<T>& rp2() {
      return pt2;
    }

    // Set points.
    inline void setPoints(const Point2D<T>& P1, const Point2D<T>& P2) {
      pt1 = P1;
      pt2 = P2;
    }

    // Set points with raw coordinates.
    inline void setPoints(T X1, T Y1, T X2, T Y2) {
      pt1 = Point2D<T>(X1, Y1);
      pt2 = Point2D<T>(X2, Y2);
    }

    // Sets starting point.
    inline void setP1(const Point2D<T>& P1) {
      pt1 = P1;
    }

    // Sets ending point.
    inline void setP2(const Point2D<T>& P2) {
      pt2 = P2;
    }

    // Translate line with a given point.
    inline void translate(const Point2D<T>& point) {
      pt1 += point;
      pt2 += point;
    }

    // Translate line with raw coordinates.
    inline void translate(T adx, T ady) {
      this->translate(Point2D<T>(adx, ady));
    }

    // Returns horizontal component of Line's vector.
    inline T dx() const {
      return pt2.x() - pt1.x();
    }

    // Returns vertical component of Line's vector.
    inline T dy() const {
      return pt2.y() - pt1.y();
    }

    // Returns Line's length.
    inline T length() const {
      return Utils::Point2D<T>::distance(pt1, pt2);
    }

    // Returns Line's length squared.
    inline T lengthSquared() const {
      return Utils::Point2D<T>::distance2(pt1, pt2);
    }

    inline Point2D<T> pointAt(T t) const {
      return Point2D<T>(pt1.x() + (pt2.x() - pt1.x()) * t,
                        pt1.y() + (pt2.y() - pt1.y()) * t);
    }

    // Returns true if a given point is on this line, with optional threshold.
    //bool isPointOnLine(Point2D<T> point, float epsilon = 0.001) {
    //  T distanceAB = this->length();
    //  T distanceAC = Utils::Point2D<T>::distance(pt1, point);
    //  T distanceBC = Utils::Point2D<T>::distance(pt2, point);
    //  return distanceAC + distanceBC - distanceAB < epsilon;
    //}

    // Returns a perpendicular line with the same starting point and length.
    inline Line<T> normalVector() const {
      return Line<T>(p1(), p1() - Point2D<T>(dy(), -dx()));
    }

    // Draw Line with OpenGL calls.
    void draw() const {
      if(smooth)
        glEnable(GL_LINE_SMOOTH);
      else
        glDisable(GL_LINE_SMOOTH);

      glLineWidth(lineWidth);
      color.setGLColor();

      glBegin(GL_LINES);
      Utils::glVertex2<T>(p1());
      Utils::glVertex2<T>(p2());
      glEnd();
    }

    // Draw Line's endpoints with OpenGL calls.
    void drawPoints() const {
      if(smoothPoints)
        glEnable(GL_POINT_SMOOTH);
      else
        glDisable(GL_POINT_SMOOTH);

      glPointSize(pointSize);
      pointsColor.setGLColor();
      glBegin(GL_POINTS);
      Utils::glVertex2<T>(pt1);
      Utils::glVertex2<T>(pt2);
      glEnd();
    }

    // Prints line to a stream.
    inline friend std::ostream& operator<<(std::ostream& os, Line<T>& line) {
      return os << "[" << line.p1() << " : " << line.p2() << "]" << std::endl;
    }

  }; // end class Line

} // end namespace Utils
