#include <GL/glut.h>
#include <iostream>
#include "Circle.h"

// Typedefs.
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Circle<GLdouble> Circle;

// Window size.
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors.
const Utils::Color bgColor(Utils::WHITE);

Circle circle(WIDTH / 2, HEIGHT / 2, 300);

void init() {
  // Background color setup.
  bgColor.setGLClearColor();

  // Display settings.
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);

  // Enable blending.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  circle.lineWidth = 2;
  circle.setPoints(4);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  circle.drawDiagonals();

  glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) {
  switch(key) {
    case 'a':
    circle++;
    glutPostRedisplay();
    break;

    case 'd':
    circle--;
    glutPostRedisplay();
    break;

    default:
    break;
  }
}

void keyUp(unsigned char key, int x, int y) {
  switch(key) {
    case 'a':
    break;

    case 'd':
    break;

    default:
    break;
  }
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 02");

  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutMainLoop();
  return 0;
}
