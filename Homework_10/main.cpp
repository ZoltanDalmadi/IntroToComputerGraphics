#include <GL/freeglut.h>
#include "Rectangle.h"
#include "Matrix.h"
#include "Point3D.h"
#include "Color.h"
#include "Vector2D.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------
typedef Utils::Rectangle<GLdouble> Rect;
typedef Utils::CentralProjection<GLdouble> CP;
typedef Utils::WindowToViewport<GLdouble> WTV;
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Point3D<GLdouble> Point3D;
typedef Utils::Point3DH<GLdouble> Point3DH;

// ----------------------------------------------------------------------------
// Window size
// ----------------------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// ----------------------------------------------------------------------------
// Colors
// ----------------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color graphColor(Utils::ORANGE);
const Utils::Color graphGridColor(Utils::BLACK);

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
// Miscellaneous variables
// ----------------------------------------------------------------------------
const double step = 0.5f;
const double xMin = -20.0f;
const double xMax = 20.0f;
const double yMin = -20.0f;
const double yMax = 20.0f;

// ----------------------------------------------------------------------------
// Window and viewport
// ----------------------------------------------------------------------------
Rect window(xMin, yMin, xMax, yMax);
Rect viewport(280, 0, 280 + HEIGHT, HEIGHT);

// ----------------------------------------------------------------------------
// Matrices
// ----------------------------------------------------------------------------
WTV wtv1(window, viewport);

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
// Main display function
// ----------------------------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glutSwapBuffers();
}

// ----------------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 10");

  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
