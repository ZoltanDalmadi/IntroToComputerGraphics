#include <GL/glut.h>
#include <iostream>
#include <random>
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
const Utils::Color ball1Color(Utils::BLUE);
const Utils::Color ball2Color(Utils::RED);

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
Point2D *c1 = ball1.pc();
Vector2D vec1(1, 1);
Circle ball1Outer(*c1, ballSize);

Circle ball2(WIDTH - 100, HEIGHT - 100, ballSize);
Point2D *c2 = ball2.pc();
Vector2D vec2(1, 1);
Circle ball2Outer(*c2, ballSize);


bool keyStates[256];
GLdouble delta = 1.0;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> disX(0, WIDTH);
std::uniform_int_distribution<> disY(0, HEIGHT);

Point2D *food1;
Point2D *food2;

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
  ball1.color = ball1Color;
  ball1.setPoints(4);
  ball2.lineWidth = 2;
  ball2.color = ball2Color;
  ball2.setPoints(4);

  ball1Outer.lineWidth = 5;
  ball2Outer.lineWidth = 5;

  food1 = new Point2D(disX(gen), disY(gen));
  food1->color = ball1Color;
  food2 = new Point2D(disX(gen), disY(gen));
  food2->color = ball2Color;
}

void keyPressed(unsigned char key, int x, int y) {
  keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
  keyStates[key] = false;
}

void keyOperations() {
  if(keyStates['a']) {
    line.rp1().changeX(-delta);
    lineVector.set(line.dx(), line.dy());
  }

  if(keyStates['d']) {
    line.rp1().changeX(delta);
    lineVector.set(line.dx(), line.dy());
  }

  if(keyStates['4']) {
    line.rp2().changeX(-delta);
    lineVector.set(line.dx(), line.dy());
  }

  if(keyStates['6']) {
    line.rp2().changeX(delta);
    lineVector.set(line.dx(), line.dy());
  }
}

void detectLineCollision() {
  if(c1->distanceToLineSquared(line) <= ballSizeSquared)
    vec1.reflectFrom(lineVector);
  if(c2->distanceToLineSquared(line) <= ballSizeSquared)
    vec2.reflectFrom(lineVector);
}

void detectWallCollision() {
  if(c1->distanceToLineSquared(leftWall) <= ballSizeSquared)
    vec1.reflectFrom(leftWallVector);

  if(c1->distanceToLineSquared(rightWall) <= ballSizeSquared)
    vec1.reflectFrom(rightWallVector);

  if(c1->distanceToLineSquared(topWall) <= ballSizeSquared)
    vec1.reflectFrom(topWallVector);

  if(c1->distanceToLineSquared(bottomWall) <= ballSizeSquared)
    vec1.reflectFrom(bottomWallVector);

  if(c2->distanceToLineSquared(leftWall) <= ballSizeSquared)
    vec2.reflectFrom(leftWallVector);

  if(c2->distanceToLineSquared(rightWall) <= ballSizeSquared)
    vec2.reflectFrom(rightWallVector);

  if(c2->distanceToLineSquared(topWall) <= ballSizeSquared)
    vec2.reflectFrom(topWallVector);

  if(c2->distanceToLineSquared(bottomWall) <= ballSizeSquared)
    vec2.reflectFrom(bottomWallVector);
}

void detectFoodCollision() {
  if(Point2D::distance2(*c1, *food1) < ballSizeSquared) {
    delete food1;
    food1 = new Point2D(disX(gen), disY(gen));
    food1->color = ball1Color;
    ball1++;
  }

  if(Point2D::distance2(*c1, *food2) < ballSizeSquared) {
    delete food2;
    food2 = new Point2D(disX(gen), disY(gen));
    food2->color = ball2Color;
    ball1--;
  }

  if(Point2D::distance2(*c2, *food1) < ballSizeSquared) {
    delete food1;
    food1 = new Point2D(disX(gen), disY(gen));
    food1->color = ball1Color;
    ball2--;
  }

  if(Point2D::distance2(*c2, *food2) < ballSizeSquared) {
    delete food2;
    food2 = new Point2D(disX(gen), disY(gen));
    food2->color = ball2Color;
    ball2++;
  }
}

void display() {
  keyOperations();
  glClear(GL_COLOR_BUFFER_BIT);

  line.draw();
  line.drawPoints();
  ball1.drawDiagonals();
  ball1Outer.draw();
  ball2.drawDiagonals();
  ball2Outer.draw();
  food1->draw();
  food2->draw();

  glutSwapBuffers();

  ball1Outer.setCentre(*c1);
  ball2Outer.setCentre(*c2);

  detectLineCollision();
  detectWallCollision();
  detectFoodCollision();

  ball1.translate(vec1);
  ball2.translate(vec2);
}

void gameUpdate(int n) {
  glutPostRedisplay();
  glutTimerFunc(refreshRate, gameUpdate, 0);
}

void cleanUp() {
  delete food1;
  delete food2;
  food1 = nullptr;
  food2 = nullptr;
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
  cleanUp();
  return 0;
}
