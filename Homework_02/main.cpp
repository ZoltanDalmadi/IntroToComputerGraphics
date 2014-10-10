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
const GLdouble ballSizeSquared = ballSize*ballSize;

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

bool keyStates[256];
GLdouble delta = 2.0;

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
  ball1.lineWidth = 3;
  ball2.lineWidth = 3;
}

void keyPressed(unsigned char key, int x, int y) {
  keyStates[key] = true;
  glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y) {
  keyStates[key] = false;
}

void keyOperations() {
  if(keyStates['a']) {
    line.rp1().changeX(-delta);
  }

  if(keyStates['d']) {
    line.rp1().changeX(delta);
  }

  if(keyStates['4']) {
    line.rp2().changeX(-delta);
  }

  if(keyStates['6']) {
    line.rp2().changeX(delta);
  }
}

void display() {
  keyOperations();
  glClear(GL_COLOR_BUFFER_BIT);

  line.draw();
  line.drawPoints();
  ball1.draw();
  ball2.draw();

  glutSwapBuffers();

  ball1.translate(vec1);
  ball2.translate(vec2);

  Point2D *c1 = &ball1.c();
  Point2D *c2 = &ball2.c();

  //std::cout << "(" << vec1.x() << ", " << vec1.y() << ")" << std::endl;
  if(Line::pDistanceToLineSquared(*c1, line) <= ballSizeSquared) {
    vec1 = Vector2D::reflectFrom(vec1, lineVector);
  }

  if(Line::pDistanceToLineSquared(*c1, leftWall) <= ballSizeSquared) {
    vec1 = Vector2D::reflectFrom(vec1, leftWallVector);
  }

  if(Line::pDistanceToLineSquared(*c1, rightWall) <= ballSizeSquared) {
    vec1 = Vector2D::reflectFrom(vec1, rightWallVector);
  }

  if(Line::pDistanceToLineSquared(*c1, topWall) <= ballSizeSquared) {
    vec1 = Vector2D::reflectFrom(vec1, topWallVector);
  }

  if(Line::pDistanceToLineSquared(*c1, bottomWall) <= ballSizeSquared) {
    vec1 = Vector2D::reflectFrom(vec1, bottomWallVector);
  }

  if(Line::pDistanceToLineSquared(*c2, line) <= ballSizeSquared) {
    vec2 = Vector2D::reflectFrom(vec2, lineVector);
  }

  if(Line::pDistanceToLineSquared(*c2, leftWall) <= ballSizeSquared) {
    vec2 = Vector2D::reflectFrom(vec2, leftWallVector);
  }

  if(Line::pDistanceToLineSquared(*c2, rightWall) <= ballSizeSquared) {
    vec2 = Vector2D::reflectFrom(vec2, rightWallVector);
  }

  if(Line::pDistanceToLineSquared(*c2, topWall) <= ballSizeSquared) {
    vec2 = Vector2D::reflectFrom(vec2, topWallVector);
  }

  if(Line::pDistanceToLineSquared(*c2, bottomWall) <= ballSizeSquared) {
    vec2 = Vector2D::reflectFrom(vec2, bottomWallVector);
  }
}

void gameUpdate(int n) {
  glutPostRedisplay();
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
