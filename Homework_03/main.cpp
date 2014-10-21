#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Slider.h"

// Window size.
const GLsizei WIDTH = 800;
const GLsizei HEIGHT = 800;
const Utils::Color bgColor(Utils::WHITE);

typedef Utils::Slider<GLint> Slider;

Slider slider(100, 400, 700, 400);

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
  slider.draw();
  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
    slider.checkClick(xMouse, yMouse, 12);
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
    slider.release();
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
  if(slider.isDragging()) {
    slider.setHandlePos(xMouse);
    std::cout << slider.getValue() << std::endl;
  }
  
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 03");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
