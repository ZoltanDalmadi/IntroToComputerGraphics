#include <GL/glut.h>
#include "Matrix.h"
#include "Color.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Matrix<GLdouble> Matrix;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;
const int gridSize = 40;

// Refresh rate ---------------------------------------------------------------
const size_t refreshRate = 5;

void init()
{
  bgColor.setGLClearColor();

  // Display settings
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawGrid(int width, int height, int gap, GLfloat lineWidth,
              const Utils::Color& color)
{
  color.setGLColor();
  glLineWidth(lineWidth);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xAAAA);
  glBegin(GL_LINES);

  for (int i = gap; i < width; i += gap)
  {
    glVertex2i(i, 0);
    glVertex2i(i, height);
  }

  for (int i = gap; i < height; i += gap)
  {
    glVertex2i(0, i);
    glVertex2i(width, i);
  }

  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
}

void appUpdate(int n)
{
  glutPostRedisplay();
  glutTimerFunc(refreshRate, appUpdate, 0);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 05");

  init();
  glutDisplayFunc(display);
  glutTimerFunc(refreshRate, appUpdate, 0);
  glutMainLoop();
  return 0;
}
