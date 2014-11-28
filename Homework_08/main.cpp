#include <GL/freeglut.h>
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

// Refresh rate ---------------------------------------------------------------
const size_t refreshRate = 10;

// ----------------------------------------------------------------------------
// Sizes
// ----------------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

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
  cube.lineWidth = lineWidth;
}

// ----------------------------------------------------------------------------
// Main display function
// ----------------------------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  cube.draw(wtv1 * pp * rx * ry);
  cube.draw(wtv2 * cp * rx * ry);
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
    std::cout << "(" << v.x() << ", " << v.y() << ")" << std::endl;
    rx.setAngle(lastRotX + Utils::degToRad(-v.y()) * 0.25);
    ry.setAngle(lastRotY + Utils::degToRad(v.x()) * 0.25);
  }

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
  glutMainLoop();
  return 0;
}
