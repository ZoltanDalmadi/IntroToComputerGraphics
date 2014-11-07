#include <GL/glut.h>
#include "Matrix.h"
#include "PolyStar.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Matrix<GLdouble> Matrix;
typedef Utils::Rotate2D<GLdouble> Rotate2D;
typedef Utils::Scale2D<GLdouble> Scale2D;
typedef Utils::PolyStar<GLdouble> PolyStar;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

// Refresh rate ---------------------------------------------------------------
const size_t refreshRate = 20;

// Stars ----------------------------------------------------------------------
PolyStar star1(WIDTH / 2, HEIGHT / 2, 6, 80, 200);
PolyStar star2(WIDTH / 2, HEIGHT / 2, 6, 200, 300);

// Matrices -------------------------------------------------------------------
Rotate2D rot1((2 * Utils::PI) / 360);
Rotate2D rot2(-(2 * Utils::PI) / 360);
Scale2D scale1(0.99);
Scale2D scale2(1/0.99);
Matrix T1(3, 3);
Matrix T2(3, 3);

// Frames of animation --------------------------------------------------------
size_t frames = 0;

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

  T1 = rot1 * scale1;
  T2 = rot2 * scale1;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  star1.draw();
  star2.draw();

  glutSwapBuffers();

  star1.transform(T1);
  star2.transform(T2);
}

void appUpdate(int n)
{
  frames++;

  if(frames == 90)
  {
    T1 = rot1 * scale2;
    T2 = rot2 * scale2;
  }

  if(frames == 180)
  {
    T1 = rot1 * scale1;
    T2 = rot2 * scale1;
    frames = 0;
  }

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
