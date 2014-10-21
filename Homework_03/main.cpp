#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Rectangle.h"

// Window size.
const GLsizei WIDTH = 800;
const GLsizei HEIGHT = 800;
const Utils::Color bgColor(Utils::WHITE);

typedef Utils::Rectangle<GLint> Rect;

Rect rect(100, 400, 200, 300);

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
  rect.draw();
  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 03");

  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
