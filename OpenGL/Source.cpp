#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "Line.h"
#include "Color.h"

// Window size.
const GLsizei winWidth = 1280;
const GLsizei winHeight = 720;

// Typedefs.
typedef Utils::Line<GLfloat> Line;
typedef Utils::Point2D<GLfloat> Point2D;
typedef std::vector<Line> lineVector;
typedef std::vector<Point2D> pointVector;

// Colors.
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color lineColor(Utils::BLACK);
const Utils::Color pointColor(Utils::DARK_BLUE);
const Utils::Color pointColor1(Utils::MAGENTA);
const Utils::Color pointColor2(Utils::DARK_CYAN);

// Sizes.
const GLfloat lineWidth = 2.0;
const GLfloat pointSize = 14.0;

// Lines container.
lineVector allLines;
pointVector allPoints;

// Pointer to dragged point.
Point2D *activePoint = nullptr;

inline void setupLines(lineVector& lines) {
  for(auto& line : lines) {
    line.color = lineColor;
    line.pointsColor = pointColor;
    line.lineWidth = lineWidth;
    line.pointSize = pointSize;
  }
}

void setPointsColor(pointVector& points) {
  for(auto& point : points) {
    auto theta = allLines.front().standardEquation(point);

    if(theta > 0) {
      point.color = pointColor1;
    }
    if(theta < 0) {
      point.color = pointColor2;
    }
  }
}

void init() {
  bgColor.setGLClearColor();
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, winWidth, 0.0, winHeight);
  glShadeModel(GL_FLAT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  allLines.emplace_back(100, 200, 400, 250);
  setupLines(allLines);

  allPoints.emplace_back(400, 600);
  allPoints.back().size = 20;
  setPointsColor(allPoints);
}

// Draw segment to lines in container.
void drawSegmentsToLines(const lineVector& lines,
                         size_t segments,
                         size_t segmentSize,
                         Utils::Color segmentColor = Utils::RED) {
  segmentColor.setGLColor();
  glPointSize(static_cast<GLfloat>(segmentSize));
  glBegin(GL_POINTS);
  for(const auto& line : lines) {
    for(size_t i = 1; i < segments; i++) {
      Point2D segPoint(line.pointAt(static_cast<GLfloat>(i) / segments));
      glVertex2f(segPoint.x(), segPoint.y());
    }
  }
  glEnd();
}

// Draw normal vector to lines in container.
void drawNormalsToLines(const lineVector& lines,
                        size_t lineWidth,
                        bool arrow = true,
                        Utils::Color normalColor = Utils::BLACK) {
  for(auto& line : lines) {
    Line norm(line.normalVector());
    norm.draw();
    //drawArrowHead(norm.p2(), normalColor);
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  // draw lines and points
  for(auto& line : allLines) {
    line.draw();
    line.drawPoints();
  }

  drawSegmentsToLines(allLines, 6, 10);
  //drawNormalsToLines(allLines, 5);

  for(auto& point : allPoints) {
    point.draw();
  }

  glutSwapBuffers();
}

Point2D* getActivePoint(lineVector& p, GLint sens, GLint x, GLint y) {
  GLint s = sens * sens;
  Point2D *point = nullptr;
  Point2D mousePosition(static_cast<GLfloat>(x), static_cast<GLfloat>(y));

  for(auto& line : p) {
    if(Point2D::distance2(line.p1(), mousePosition) < s) {
      point = line.pp1();
    }

    if(Point2D::distance2(line.p2(), mousePosition) < s) {
      point = line.pp2();
    }
  }

  return point;
}

Point2D* getActivePoint(pointVector& p, GLint sens, GLint x, GLint y) {
  GLint s = sens * sens;
  Point2D *point = nullptr;
  Point2D mousePosition(static_cast<GLfloat>(x), static_cast<GLfloat>(y));

  for(auto& pt : p) {
    if(Point2D::distance2(pt, mousePosition) < s) {
      point = &pt;
    }
  }

  return point;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
    activePoint = getActivePoint(allLines, 10, xMouse, winHeight - yMouse);
    if(!activePoint) {
      activePoint = getActivePoint(allPoints, 10, xMouse, winHeight - yMouse);
    }
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
    activePoint = nullptr;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
  if(!activePoint)
    return;

  activePoint->setX(static_cast<GLfloat>(xMouse));
  activePoint->setY(static_cast<GLfloat>(winHeight - yMouse));
  setPointsColor(allPoints);
  glutPostRedisplay();
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(winWidth, winHeight);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Drag & Drop");
  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
