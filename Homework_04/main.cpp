#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Color.h"
#include "Matrix.h"

// Window size.
const GLsizei WIDTH = 720;
const GLsizei HEIGHT = 720;

// Colors.
const Utils::Color bgColor(Utils::WHITE);

GLdouble fos[16] = { 1, 5,  9, 13,
                     2, 6, 10, 14,
                     3, 7, 11, 15,
                     4, 8, 12, 16 };

GLdouble kaka[16] = { 17, 21, 25, 29,
                      18, 22, 26, 30,
                      19, 23, 27, 31,
                      20, 24, 28, 32 };

Utils::Matrix<4, 4, GLdouble> mat1(fos);
Utils::Matrix<4, 4, GLdouble> mat2(kaka);

void init() {
  bgColor.setGLClearColor();

  // Display settings
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  std::cout << mat1(1, 0) << std::endl;
  mat1.print(std::cout);
  (mat1 * mat2).print(std::cout);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 04");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
