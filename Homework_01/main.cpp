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
typedef std::vector<Point2D> pointVector;

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
const Utils::Color pointColor(Utils::RED);

// Sizes.
const GLfloat lineWidth = 4.0;
const GLfloat pointSize = 14.0;

// Points container.
pointVector allPoints;

// Define main circle.
Circle mainCircle(CENTER, RADIUS);

// Points (segments) of main circle.
size_t points = 256;

// Length of hands.
GLint bigHandLength = RADIUS - 10;
GLint smallHandLength = RADIUS - 110;

// Rotation of hands.
GLdouble bigHandRot = 90.0;
GLdouble smallHandRot = 0.0;

// Define hands.
Line bigHand(CENTER.x(), CENTER.y(), CENTER.x(), CENTER.y() + bigHandLength);
Line smallHand(CENTER.x(), CENTER.y(), CENTER.x() + smallHandLength, CENTER.y());

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

  // Setup colors
  mainCircle.color = lineColor;
  mainCircle.pointColor = pointColor;

  // Setup line widths.
  mainCircle.lineWidth = lineWidth;
  bigHand.lineWidth = lineWidth;
  smallHand.lineWidth = lineWidth;
}

void drawThing(int width, int height, int x, int y, int gap, GLfloat lineWidth,
               GLubyte* color) {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3ubv(color);
  glLineWidth(lineWidth);
  glBegin(GL_LINES);

  // draw axis
  glVertex2i(x, y);
  glVertex2i(x, y + height);
  glVertex2i(x, y);
  glVertex2i(x + width, y);

  size_t notches = height / gap;

  for(size_t i = 0; i < notches; i++) {
    glVertex2i(x, y + gap*i);
    glVertex2i(x + width - gap*i, y);
  }

  glEnd();
  glFlush();
}

void clockDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw Circle.
  mainCircle.draw();
  mainCircle.drawPoints(12);

  // Draw hands.
  bigHand.draw();
  smallHand.draw();

  // Draw center point.
  mainCircle.drawCenterPoint();

  glutSwapBuffers();
}

void clockUpdate(int n) {
  GLdouble rot = degToRad(bigHandRot);
  bigHand.setP2(CENTER.x() + bigHandLength * cos(rot),
                CENTER.y() + bigHandLength * sin(rot));

  rot = degToRad(smallHandRot);
  smallHand.setP2(CENTER.x() + smallHandLength * cos(rot),
                  CENTER.y() + smallHandLength * sin(rot));

  bigHandRot--;
  smallHandRot -= 1.0 / 12.0;

  glutPostRedisplay();
  glutTimerFunc(50, clockUpdate, 0);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 01");

  init();
  glutDisplayFunc(clockDisplay);
  glutTimerFunc(50, clockUpdate, 0);
  glutMainLoop();
  return 0;
}