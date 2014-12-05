#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Rectangle.h"
#include "Point3D.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------
typedef Utils::Rectangle<GLdouble> Rect;
typedef Utils::WindowToViewport<GLdouble> WTV;

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
// Window and viewport
// ----------------------------------------------------------------------------
Rect window(-1, -1, 1, 1);
Rect viewport(0, 40, 0 + 640, 40 + 640);

// ----------------------------------------------------------------------------
// Matrices
// ----------------------------------------------------------------------------
WTV wtv1(window, viewport);

// ----------------------------------------------------------------------------
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;

// ----------------------------------------------------------------------------
// Miscellaneous variables
// ----------------------------------------------------------------------------
double p = 0.0;


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
// Info test function. Shows current value of p
// ----------------------------------------------------------------------------
void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "p: " << p << std::endl;
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
  glutCreateWindow("Homework 09");

  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
