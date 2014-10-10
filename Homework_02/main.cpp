#include <GL/glut.h>
#include <iostream>
#include <map>
#include "Line.h"
#include "Circle.h"

// Typedefs.
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Circle<GLdouble> Circle;
typedef Utils::Line<GLdouble> Line;

// Window size.
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors.
const Utils::Color bgColor(Utils::WHITE);

Line line(300, HEIGHT, 800, 0);
Line line2(100, 260, 100, 400);
Line line3(100, 260, 200, 460);

std::map<unsigned char, bool> keyStates {
    { 'a', false },
    { 'd', false },
    { '4', false },
    { '6', false }
};

GLdouble delta = 0.05;

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

  line.lineWidth = 2;
}

void keyPressed(unsigned char key, int x, int y) {
  keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
  keyStates[key] = false;
}

void keyOperations() {
  if(keyStates['a']) { line.rp1().changeX(-delta); }
  if(keyStates['d']) { line.rp1().changeX(delta); }

  if(keyStates['4']) { line.rp2().changeX(-delta); }
  if(keyStates['6']) { line.rp2().changeX(delta); }

  glutPostRedisplay();
}

void display() {
  keyOperations();
  glClear(GL_COLOR_BUFFER_BIT);

  line.draw();
  line.drawPoints();

  line2.draw();
  line2.drawPoints();
  line3.draw();
  line3.drawPoints();

  glutSwapBuffers();
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
