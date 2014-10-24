#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Slider.h"
#include "Circle.h"

// Typedefs.
typedef Utils::Slider Slider;
typedef Utils::Circle<GLdouble> Circle;

// Window size.
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Constants.
const size_t revolutions = 5;
const size_t circlePoints = 48;
const double delta = static_cast<double>(circlePoints) * revolutions / 100;

const size_t maxEvolvents = 30;

// Colors.
const Utils::Color bgColor(Utils::WHITE);

// Items.
Circle circ(WIDTH / 2, HEIGHT / 2, 10, circlePoints);
Slider progressSlider(100, 40, 1180, 40);
Slider numbersSlider(100, 80, 1180, 80);
Slider radiusSlider(100, 120, 1180, 120);

void init() {
  bgColor.setGLClearColor();

  // Display settings
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  circ.lineWidth = 2;
  radiusSlider.setValue(10);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  circ.draw();
  size_t asd = static_cast<size_t>(1 + (numbersSlider.getValue() *
                                        (maxEvolvents - 1) / 100));
  circ.drawEvolvents(asd, progressSlider.getValue() * delta);
  progressSlider.draw();
  numbersSlider.draw();
  radiusSlider.draw();
  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
    progressSlider.checkClick(xMouse, HEIGHT - yMouse, 12);
    numbersSlider.checkClick(xMouse, HEIGHT - yMouse, 12);
    radiusSlider.checkClick(xMouse, HEIGHT - yMouse, 12);
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
    progressSlider.release();
    numbersSlider.release();
    radiusSlider.release();
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
  if (progressSlider.isDragging()) {
    progressSlider.setHandlePos(xMouse);
  }

  if (numbersSlider.isDragging()) {
    numbersSlider.setHandlePos(xMouse);
  }

  if (radiusSlider.isDragging()) {
    radiusSlider.setHandlePos(xMouse);
    circ.setRadius(radiusSlider.getValue());
  }

  glutPostRedisplay();
}

int main(int argc, char **argv) {
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
