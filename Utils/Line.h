#pragma once

#include <GL/freeglut.h>
#include <cmath>
#include "Point2D.h"
#include "Color.h"

namespace Utils {

  // class Line<T> definition
  // =========================================================================

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

    Line(T a1, T a2, T b1, T b2);
    Line(Point2D<T> a, Point2D<T> b);

    inline T x1() const;
    inline T x2() const;
    inline T y1() const;
    inline T y2() const;

    inline Point2D<T> p1() const;
    inline Point2D<T> p2() const;
    inline Point2D<T>* pp1();
    inline Point2D<T>* pp2();
    inline Point2D<T>& rp1();
    inline Point2D<T>& rp2();

    inline void setPoints(const Point2D<T>& P1, const Point2D<T>& P2);
    inline void setPoints(T X1, T Y1, T X2, T Y2);
    inline void setP1(const Point2D<T>& P1);
    inline void setP2(const Point2D<T>& P2);
    inline void translate(const Point2D<T>& point);
    inline void translate(T adx, T ady);

    inline T dx() const;
    inline T dy() const;
    inline T length() const;
    inline T lengthSquared() const;
    inline Point2D<T> pointAt(T t) const;

    inline Line<T> normalVector() const;
    void draw() const;
    void drawPoints() const;

    inline friend std::ostream& operator<<(std::ostream& os, Line<T>& line);

  }; // end class Line

    // class Line<T> implementation
    // =======================================================================

    /// Init Line with raw coordinates.
    template <typename T>
    inline Line<T>::Line(T a1, T a2, T b1, T b2) : pt1(a1, a2), pt2(b1, b2) {}

    /// Init Line with existing points.
    template <typename T>
    inline Line<T>::Line(Point2D<T> a, Point2D<T> b) : pt1(a), pt2(b) {}

    /// Returns starting point's X coordinate.
    template <typename T>
    inline T Line<T>::x1() const {
      return pt1.x();
    }

    /// Returns ending point's X coordinate.
    template <typename T>
    inline T Line<T>::x2() const {
      return pt2.x();
    }

    /// Returns starting point's Y coordinate.
    template <typename T>
    inline T Line<T>::y1() const {
      return pt1.y();
    }

    /// Returns ending point's Y coordinate.
    template <typename T>
    inline T Line<T>::y2() const {
      return pt2.y();
    }

    /// Returns starting point of Line.
    template <typename T>
    inline Point2D<T> Line<T>::p1() const {
      return pt1;
    }

    /// Returns ending point of Line.
    template <typename T>
    inline Point2D<T> Line<T>::p2() const {
      return pt2;
    }

    /// Returns a pointer to starting point of Line.
    template <typename T>
    inline Point2D<T>* Line<T>::pp1() {
      return pt1.getPointer();
    }

    /// Returns a pointer to ending point of Line.
    template <typename T>
    inline Point2D<T>* Line<T>::pp2() {
      return pt2.getPointer();
    }

    /// Returns a reference to starting point of Line.
    template <typename T>
    inline Point2D<T>& Line<T>::rp1() {
      return pt1;
    }

    /// Returns a reference to ending point of Line.
    template <typename T>
    inline Point2D<T>& Line<T>::rp2() {
      return pt2;
    }

    /// Set points.
    template <typename T>
    inline void Line<T>::setPoints(const Point2D<T>& P1, const Point2D<T>& P2) {
      pt1 = P1;
      pt2 = P2;
    }

    /// Set points with raw coordinates.
    template <typename T>
    inline void Line<T>::setPoints(T X1, T Y1, T X2, T Y2) {
      pt1 = Point2D<T>(X1, Y1);
      pt2 = Point2D<T>(X2, Y2);
    }

    /// Sets starting point.
    template <typename T>
    inline void Line<T>::setP1(const Point2D<T>& P1) {
      pt1 = P1;
    }

    /// Sets ending point.
    template <typename T>
    inline void Line<T>::setP2(const Point2D<T>& P2) {
      pt2 = P2;
    }

    /// Translate line with a given point.
    template <typename T>
    inline void Line<T>::translate(const Point2D<T>& point) {
      pt1 += point;
      pt2 += point;
    }

    /// Translate line with raw coordinates.
    template <typename T>
    inline void Line<T>::translate(T adx, T ady) {
      this->translate(Point2D<T>(adx, ady));
    }

    /// Returns horizontal component of Line's vector.
    template <typename T>
    inline T Line<T>::dx() const {
      return x2() - x1();
    }

    /// Returns vertical component of Line's vector.
    template <typename T>
    inline T Line<T>::dy() const {
      return y2() - y1();
    }

    /// Returns Line's length.
    template <typename T>
    inline T Line<T>::length() const {
      return Utils::Point2D<T>::distance(pt1, pt2);
    }

    /// Returns Line's length squared.
    template <typename T>
    inline T Line<T>::lengthSquared() const {
      return Utils::Point2D<T>::distance2(pt1, pt2);
    }

    /// Returns a Point2D at a position between pt1 and pt2 in [0, 1] range.
    template <typename T>
    inline Point2D<T> Line<T>::pointAt(T t) const {
      return Point2D<T>(pt1.x() + (pt2.x() - pt1.x()) * t,
                        pt1.y() + (pt2.y() - pt1.y()) * t);
    }

    /// Returns a perpendicular line with the same starting point and length.
    template <typename T>
    inline Line<T> Line<T>::normalVector() const {
      return Line<T>(pt1, pt1 + Point2D<T>(-dy(), dx()));
    }

    /// Draw Line with OpenGL calls.
    template <typename T>
    void Line<T>::draw() const {
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

    /// Draw Line's endpoints with OpenGL calls.
    template <typename T>
    void Line<T>::drawPoints() const {
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

    /// Prints line to a stream.
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Line<T>& line) {
      return os << "[" << line.p1() << " : " << line.p2() << "]" << std::endl;
    }

} // end namespace Utils
