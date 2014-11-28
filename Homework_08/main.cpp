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
typedef Utils::PerpendicularProjection<GLdouble> PP;
typedef Utils::CentralProjection<GLdouble> CP;
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
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;


Rect window(-1, -1, 1, 1);
Rect viewport1(0, 40, 0 + 640, 40 + 640);
Rect viewport2(640, 40, 640 + 640, 40 + 640);
Cube cube;

PP pp;
CP cp(2.0f);
WTV wtv1(window, viewport1);
WTV wtv2(window, viewport2);
Rotate3DX rx(Utils::degToRad(0));
Rotate3DY ry(Utils::degToRad(0));

bool drag = false;

double lastRotX = 0.0f;
double lastRotY = 0.0f;

GLint clickedX;
GLint clickedY;

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
// Info test function. Shows current value of t
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
// Draw grid to background
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
    //glVertex2i(i, 0);
    //glVertex2i(i, height);
    Utils::Point3DH<GLdouble> asd1(i, -0.5, end);
    Utils::glVertex2<GLdouble>(asd1.transformed(mat).normalized2D());
    Utils::Point3DH<GLdouble> asd2(i, -0.5, start);
    Utils::glVertex2<GLdouble>(asd2.transformed(mat).normalized2D());
  }

  for (double i = start; i <= end; i += gap)
  {
    //glVertex2i(0, i);
    //glVertex2i(width, i);
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

  // draw grid to into background
  drawGrid(-0.7, 0.7, 0.1, lineWidth, Utils::VERY_LIGHT_GRAY, m1);
  drawGrid(-0.7, 0.7, 0.1, lineWidth, Utils::VERY_LIGHT_GRAY, m2);

  // draw info text
  drawInfoText(WIDTH - 60, HEIGHT - 30, Utils::BLACK);

  //Utils::Point3DH<GLdouble> p1(0, 0, 2);
  //Utils::Point3DH<GLdouble> p2(0, 0, -2);

  //glBegin(GL_LINES);
  //Utils::glVertex2<GLdouble>(p1.transformed(m1).normalized2D());
  //Utils::glVertex2<GLdouble>(p2.transformed(m1).normalized2D());
  //glEnd();

  //glBegin(GL_LINES);
  //Utils::glVertex2<GLdouble>(p1.transformed(m2).normalized2D());
  //Utils::glVertex2<GLdouble>(p2.transformed(m2).normalized2D());
  //glEnd();

  // draw cube(s)
  cube.draw(m1);
  cube.drawPoints(m1);
  cube.draw(m2);
  cube.drawPoints(m2);

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
    clickedX = xMouse;
    clickedY = HEIGHT - yMouse;
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    drag = false;
    lastRotX = rx.getAngle();
    lastRotY = ry.getAngle();
  }
}

// ----------------------------------------------------------------------------
// Main mouse movement handler
// ----------------------------------------------------------------------------
void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (drag)
  {
    Utils::Vector2D<GLdouble> v(clickedX, clickedY, xMouse, HEIGHT - yMouse);
    rx.setAngle(lastRotX + Utils::degToRad(-v.y()) * 0.25);
    ry.setAngle(lastRotY + Utils::degToRad(v.x()) * 0.25);
  }

  glutPostRedisplay();
}

void wheelFunc(int wheel, int direction, int x, int y)
{
  auto value = cp.getDistanceToOrigin();
  cp.setDistanceToOrigin(value + direction * 0.1);
  glutPostRedisplay();
}

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
