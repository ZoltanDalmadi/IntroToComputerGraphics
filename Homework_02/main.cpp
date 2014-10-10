#include <GL/glut.h>
#include <iostream>
#include <map>
#include "Line.h"
#include "Vector2D.h"
#include "Circle.h"

// Typedefs.
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Circle<GLdouble> Circle;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Vector2D<GLdouble> Vector2D;

// Window size.
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors.
const Utils::Color bgColor(Utils::WHITE);

const GLdouble ballSize = 50.0;

Line line(300, HEIGHT, 800, 0);
Line leftWall(0, 0, 0, HEIGHT);
Line rightWall(WIDTH, 0, WIDTH, HEIGHT);
Line topWall(0, HEIGHT, WIDTH, HEIGHT);
Line bottomWall(0, 0, WIDTH, 0);
Vector2D lineVector(line.dx(), line.dy());
Vector2D leftWallVector(leftWall.dx(), leftWall.dy());
Vector2D rightWallVector(rightWall.dx(), rightWall.dy());
Vector2D topWallVector(topWall.dx(), topWall.dy());
Vector2D bottomWallVector(bottomWall.dx(), bottomWall.dy());
Circle ball1(100, 100, ballSize);
Circle ball2(WIDTH - 100, HEIGHT - 100, ballSize);
Vector2D vec1(1, 1);
Vector2D vec2(1, 1);

std::map<unsigned char, bool> keyStates {
    { 'a', false },
    { 'd', false },
    { '4', false },
    { '6', false }
};

GLdouble delta = 0.05;

// Refresh rate in miliseconds.
size_t refreshRate = 5;

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
  ball1.lineWidth = 2;
  ball2.lineWidth = 2;
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
  ball1.draw();
  ball2.draw();

  glutSwapBuffers();
}

void gameUpdate(int n) {
  ball1.translate(vec1);
  if(Line::pDistanceToLine(ball1.c(), line) <= ballSize) {
    vec1 = Vector2D::reflectFrom(vec1, lineVector);
  }

  if(Line::pDistanceToLine(ball1.c(), leftWall) <= ballSize) {
    vec1 = Vector2D::reflectFrom(vec1, leftWallVector);
  }

  if(Line::pDistanceToLine(ball1.c(), rightWall) <= ballSize) {
    vec1 = Vector2D::reflectFrom(vec1, rightWallVector);
  }

  if(Line::pDistanceToLine(ball1.c(), topWall) <= ballSize) {
    vec1 = Vector2D::reflectFrom(vec1, topWallVector);
  }

  if(Line::pDistanceToLine(ball1.c(), bottomWall) <= ballSize) {
    vec1 = Vector2D::reflectFrom(vec1, bottomWallVector);
  }

  ball2.translate(vec2);
  if(Line::pDistanceToLine(ball2.c(), line) <= ballSize) {
    vec2 = Vector2D::reflectFrom(vec2, lineVector);
  }

  if(Line::pDistanceToLine(ball2.c(), leftWall) <= ballSize) {
    vec2 = Vector2D::reflectFrom(vec2, leftWallVector);
  }

  if(Line::pDistanceToLine(ball2.c(), rightWall) <= ballSize) {
    vec2 = Vector2D::reflectFrom(vec2, rightWallVector);
  }

  if(Line::pDistanceToLine(ball2.c(), topWall) <= ballSize) {
    vec2 = Vector2D::reflectFrom(vec2, topWallVector);
  }

  if(Line::pDistanceToLine(ball2.c(), bottomWall) <= ballSize) {
    vec2 = Vector2D::reflectFrom(vec2, bottomWallVector);
  }
  //std::cout << "(" << vec.x() << ", " << vec.y() << ")" << std::endl;

  glutTimerFunc(refreshRate, gameUpdate, 0);
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
  glutTimerFunc(refreshRate, gameUpdate, 0);
  glutMainLoop();
  return 0;
}
