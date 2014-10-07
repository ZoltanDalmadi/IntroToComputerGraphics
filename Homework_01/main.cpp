#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "Line.h"
#include "Circle.h"

// Typedefs.
typedef Utils::Line<GLdouble> Line;
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Circle<GLdouble> Circle;

// Window size.
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Center point.
const Point2D CENTER(WIDTH / 2, HEIGHT / 2);

// Radius of main circle.
const GLint RADIUS = 300;

// Colors.
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color lineColor(Utils::BLACK);
const Utils::Color gridColor("f08422");
const Utils::Color pointColor(Utils::RED);
const Utils::Color plateColor("fafabb");

// Sizes.
const GLfloat lineWidth = 4.0;
const GLfloat gridLineWidth = 2.0;
const GLfloat pointSize = 14.0;

// Define main circle.
Circle mainCircle(CENTER, RADIUS);
Circle dotsCircle(CENTER, RADIUS - 10);
Circle secondaryCircle(CENTER, RADIUS - 20);

// Points (segments) of main circle.
size_t points = 256;

// Length of hands.
GLint bigHandLength = RADIUS - 10;
GLint smallHandLength = RADIUS - 110;

// Rotation of hands.
GLdouble bigHandRot = 90.0;
GLdouble smallHandRot = 0.0;

// Number of hand segments.
size_t handSegments = 16;

// Define hands.
Line bigHand(CENTER.x(), CENTER.y(), CENTER.x(), CENTER.y() + bigHandLength);
Line smallHand(CENTER.x(), CENTER.y(), CENTER.x() + smallHandLength, CENTER.y());

// Refresh rate in miliseconds.
size_t refreshRate = 50;

// Degrees to radians.
GLdouble degToRad(GLdouble deg) {
  return deg * (Utils::PI / 180);
}

void init() {
  // Background color setup.
  bgColor.setGLClearColor();

  // Display settings.
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glShadeModel(GL_FLAT);

  // Line and point settings.
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);

  // Enable blending.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Setup main circle points.
  mainCircle.points = points;
  mainCircle.pointSize = pointSize;
  dotsCircle.points = points;
  dotsCircle.pointSize = pointSize;
  secondaryCircle.points = points;

  // Setup colors
  mainCircle.color = lineColor;
  mainCircle.centrePointColor = pointColor;
  dotsCircle.pointColor = pointColor;
  secondaryCircle.color = plateColor;
  secondaryCircle.filled = true;

  // Setup line widths.
  mainCircle.lineWidth = lineWidth;
  bigHand.lineWidth = lineWidth;
  smallHand.lineWidth = lineWidth;
}

// Draw grid function.
void drawGrid() {
  glLineWidth(gridLineWidth);
  gridColor.setGLColor();

  glBegin(GL_LINES);

  for(size_t i = 1; i < handSegments; i++) {
    double seg = static_cast<double>(i) / handSegments;
    Utils::glVertex2<GLdouble>(bigHand.pointAt(seg));
    seg = static_cast<double>(handSegments - i) / handSegments;
    Utils::glVertex2<GLdouble>(smallHand.pointAt(seg));
  }

  glEnd();
}

// Main display function.
void clockDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw Circle.
  mainCircle.draw();
  dotsCircle.drawPoints(12);
  secondaryCircle.draw();

  // Draw grid between hands.
  drawGrid();

  // Draw hands.
  bigHand.draw();
  smallHand.draw();

  // Draw center point.
  mainCircle.drawCenterPoint();

  glutSwapBuffers();
}

void clockUpdate(int n) {
  // Calculate new position of big hands endpoint.
  GLdouble rot = degToRad(bigHandRot);
  bigHand.setP2(CENTER.x() + bigHandLength * cos(rot),
                CENTER.y() + bigHandLength * sin(rot));

  // Same for small hand
  rot = degToRad(smallHandRot);
  smallHand.setP2(CENTER.x() + smallHandLength * cos(rot),
                  CENTER.y() + smallHandLength * sin(rot));

  // Adjust their rotation degree.
  bigHandRot--;
  smallHandRot -= 1.0 / 12.0;
  std::cout << smallHandRot << std::endl;

  // reset degrees if one rotation has passed
  if(bigHandRot == -270.0) {
    bigHandRot = 90.0;
  }

  glutPostRedisplay();
  glutTimerFunc(refreshRate, clockUpdate, 0);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Homework 01");

  init();
  glutDisplayFunc(clockDisplay);
  glutTimerFunc(refreshRate, clockUpdate, 0);
  glutMainLoop();
  return 0;
}