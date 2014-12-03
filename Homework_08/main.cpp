#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Rectangle.h"
#include "Cube.h"
#include "Vector2D.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------
typedef Utils::Rectangle<GLdouble> Rect;
typedef Utils::Cube<GLdouble> Cube;
typedef Utils::PerpendicularProjection<GLdouble> PerpendicularProjection;
typedef Utils::CentralProjection<GLdouble> CentralProjection;
typedef Utils::WindowToViewport<GLdouble> WTV;
typedef Utils::Rotate3DX<GLdouble> Rotate3DX;
typedef Utils::Rotate3DY<GLdouble> Rotate3DY;

// ----------------------------------------------------------------------------
// Window size
// ----------------------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// ----------------------------------------------------------------------------
// Colors
// ----------------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// ----------------------------------------------------------------------------
// Refresh rate
// ----------------------------------------------------------------------------
const size_t refreshRate = 10;

// ----------------------------------------------------------------------------
// Sizes
// ----------------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;
const int gridSize = 40;

// ----------------------------------------------------------------------------
// Window and viewports
// ----------------------------------------------------------------------------
Rect window(-1, -1, 1, 1);
Rect viewport1(0, 40, 0 + 640, 40 + 640);
Rect viewport2(640, 40, 640 + 640, 40 + 640);

// ----------------------------------------------------------------------------
// Matrices
// ----------------------------------------------------------------------------
PerpendicularProjection pp;
CentralProjection cp(2.0f);
WTV wtv1(window, viewport1);
WTV wtv2(window, viewport2);
Rotate3DX rx(0);
Rotate3DY ry(0);

// ----------------------------------------------------------------------------
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;

// ----------------------------------------------------------------------------
// The cube
// ----------------------------------------------------------------------------
Cube cube;

// ----------------------------------------------------------------------------
// Miscellaneous variables
// ----------------------------------------------------------------------------
bool drag = false;
double lastRotX = 0.0f;
double lastRotY = 0.0f;
GLint clickedX;
GLint clickedY;
GLint draggedX;
GLint draggedY;

// ----------------------------------------------------------------------------
// Init function
// ----------------------------------------------------------------------------
void init()
{
  bgColor.setGLClearColor();
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// ----------------------------------------------------------------------------
// Info test function. Shows current value of s
// ----------------------------------------------------------------------------
void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "s: " << cp.getDistanceToOrigin() << std::endl;
  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

// ----------------------------------------------------------------------------
// Draw grid floor
// ----------------------------------------------------------------------------
void drawGrid(double start, double end, double gap, GLfloat lineWidth,
              const Utils::Color& color, const Utils::Matrix<GLdouble>& mat)
{
  color.setGLColor();
  glLineWidth(lineWidth);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xAAAA);
  glBegin(GL_LINES);

  for (double i = start; i <= end; i += gap)
  {
    Utils::Point3DH<GLdouble> asd1(i, -0.5, end);
    Utils::glVertex2<GLdouble>(asd1.transformed(mat).normalized2D());
    Utils::Point3DH<GLdouble> asd2(i, -0.5, start);
    Utils::glVertex2<GLdouble>(asd2.transformed(mat).normalized2D());
  }

  for (double i = start; i <= end; i += gap)
  {
    Utils::Point3DH<GLdouble> asd1(start, -0.5, i);
    Utils::glVertex2<GLdouble>(asd1.transformed(mat).normalized2D());
    Utils::Point3DH<GLdouble> asd2(end, -0.5, i);
    Utils::glVertex2<GLdouble>(asd2.transformed(mat).normalized2D());
  }

  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

// ----------------------------------------------------------------------------
// Main display function
// ----------------------------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // precalculate matrices
  auto projTrans1 = wtv1 * pp;
  auto projTrans2 = wtv2 * cp;
  auto rxry = rx * ry;
  auto m1 = projTrans1 * rxry;
  auto m2 = projTrans2 * rxry;

  // draw grid floor
  drawGrid(-0.7, 0.7, 0.1, lineWidth, Utils::VERY_LIGHT_GRAY, m1);
  drawGrid(-0.7, 0.7, 0.1, lineWidth, Utils::VERY_LIGHT_GRAY, m2);

  // draw info text
  drawInfoText(WIDTH - 60, HEIGHT - 30, Utils::BLACK);

  // draw cube(s)
  cube.drawEdges(m1);
  cube.drawPoints(m1);
  cube.drawEdges(m2);
  cube.drawPoints(m2);

  if (drag)
  {
    glBegin(GL_LINES);
    glVertex2i(clickedX, clickedY);
    glVertex2i(draggedX, draggedY);
    glEnd();
  }

  glutSwapBuffers();
}

// ----------------------------------------------------------------------------
// Main mouse button handler
// ----------------------------------------------------------------------------
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    drag = true;

    // cache clicked point position
    clickedX = xMouse;
    clickedY = HEIGHT - yMouse;
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    drag = false;

    // cache current rotation values
    lastRotX = rx.getAngle();
    lastRotY = ry.getAngle();

    glutPostRedisplay();
  }
}

// ----------------------------------------------------------------------------
// Main mouse movement handler
// ----------------------------------------------------------------------------
void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (drag)
  {
    // calculate vector between clicked and dragged point
    Utils::Vector2D<GLint> v(clickedX, clickedY, xMouse, HEIGHT - yMouse);

    // set rotation values
    rx.setAngle(lastRotX - Utils::degToRad(v.y()) * 0.25);
    ry.setAngle(lastRotY + Utils::degToRad(v.x()) * 0.25);
    draggedX = clickedX + v.x();
    draggedY = clickedY + v.y();
  }

  glutPostRedisplay();
}

// ----------------------------------------------------------------------------
// Mouse wheel handler
// ----------------------------------------------------------------------------
void wheelFunc(int wheel, int direction, int x, int y)
{
  auto value = cp.getDistanceToOrigin();
  cp.setDistanceToOrigin(value + direction * 0.1);
  glutPostRedisplay();
}

// ----------------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 08");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMouseWheelFunc(wheelFunc);
  glutMainLoop();
  return 0;
}
