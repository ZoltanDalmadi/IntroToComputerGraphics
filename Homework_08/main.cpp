#include <GL/freeglut.h>
#include "Color.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------

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
// Sizes
// ----------------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

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
// Main mouse button handler
// ----------------------------------------------------------------------------
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
  }
}

// ----------------------------------------------------------------------------
// Main mouse movement handler
// ----------------------------------------------------------------------------
void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
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
