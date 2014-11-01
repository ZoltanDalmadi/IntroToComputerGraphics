#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Line.h"
#include "Matrix.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Matrix<4, 4, GLdouble> Matrix4x4;
typedef Utils::Matrix<4, 3, GLdouble> Matrix3x4;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// Points ---------------------------------------------------------------------
Point2D P1(100, 150);
Point2D P2(300, 150);
Point2D P3(800, 150);
Point2D P4(1000, 150);

GLdouble Gdata[16] = {-1, 0, 1, 8,
                       1, 0, 1, 4,
                      -1, 0, 1, 2,
                       1, 1, 1, 1 };
GLdouble Mdata[16] = {-10, 8, 9, 4,
                        0, -3, -5, -2,
                        3, 4, -3, -5,
                       -2, -3, 6, 6 };

Matrix3x4 G(Gdata);
Matrix4x4 M(Mdata);

void init() {
  bgColor.setGLClearColor();

  // Display settings
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  P1.draw();
  P2.draw();
  P3.draw();
  P4.draw();
  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
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
