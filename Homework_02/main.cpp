// Includes -------------------------------------------------------------------
#include <GL/glut.h>
#include <iostream>
#include <random>
#include "Line.h"
#include "Vector2D.h"
#include "Circle.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Circle<GLdouble> Circle;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Vector2D<GLdouble> Vector2D;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 700;
const GLsizei HEIGHT = 500;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color ball1Color(Utils::BLUE);
const Utils::Color ball2Color(Utils::RED);

// Refresh rate ---------------------------------------------------------------
const size_t refreshRate = 5;

// Movement rate of the bat ---------------------------------------------------
const GLdouble delta = 1.0;

// Ball sizes -----------------------------------------------------------------
const GLdouble ballSize = 50.0;
const GLdouble ballSizeSquared = ballSize * ballSize;

// Balls ----------------------------------------------------------------------
Circle ball1(ballSize + 10, ballSize + 10, ballSize);
Point2D *c1 = ball1.pc();              // pointer to center point of ball1
Vector2D vec1(1, 1);                   // starting movement vector of ball1
Circle ball1Outer(*c1, ballSize);      // outer circle of ball1

Circle ball2(WIDTH - ballSize - 10, HEIGHT - ballSize - 10, ballSize);
Point2D *c2 = ball2.pc();              // pointer to center point of ball2
Vector2D vec2(-1, -1);                 // starting movement vector of ball2
Circle ball2Outer(*c2, ballSize);      // outer circle of ball2

// The bat controlled by the players ------------------------------------------
Line line(WIDTH / 2, HEIGHT, WIDTH / 2, 0);
Vector2D lineVector(line.dx(), line.dy());

// Walls ----------------------------------------------------------------------
Line leftWall(0, 0, 0, HEIGHT);
Line rightWall(WIDTH, 0, WIDTH, HEIGHT);
Line topWall(0, HEIGHT, WIDTH, HEIGHT);
Line bottomWall(0, 0, WIDTH, 0);
Vector2D leftWallVector(leftWall.dx(), leftWall.dy());
Vector2D rightWallVector(rightWall.dx(), rightWall.dy());
Vector2D topWallVector(topWall.dx(), topWall.dy());
Vector2D bottomWallVector(bottomWall.dx(), bottomWall.dy());

// Random number generator ----------------------------------------------------
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> disX(static_cast<int>(ballSize),
                                        static_cast<int>(WIDTH - ballSize));
std::uniform_int_distribution<int> disY(static_cast<int>(ballSize),
                                        static_cast<int>(HEIGHT - ballSize));

// Food -----------------------------------------------------------------------
Point2D food1(disX(gen), disY(gen));
Point2D food2(disX(gen), disY(gen));

// Keyboard button states -----------------------------------------------------
bool keyStates[256];

// Init function --------------------------------------------------------------
void init()
{
  // Background color setup
  bgColor.setGLClearColor();

  // Display settings
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Init objects
  line.lineWidth = 2;

  ball1.lineWidth = 2;
  ball1.color = ball1Color;
  ball1.setPoints(7);

  ball2.lineWidth = 2;
  ball2.color = ball2Color;
  ball2.setPoints(7);

  ball1Outer.lineWidth = 5;
  ball2Outer.lineWidth = 5;
  ball1Outer.color = ball1Color;
  ball2Outer.color = ball2Color;

  food1.color = ball1Color;
  food2.color = ball2Color;
}

// Keyboard handling ----------------------------------------------------------
void keyPressed(unsigned char key, int x, int y)
{
  keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
  keyStates[key] = false;
}

void keyOperations()
{
  if (keyStates['a'])
  {
    // if on top wall
    if (line.x1() >= 0 && line.x1() <= WIDTH && line.y1() == HEIGHT)
    {
      line.rp1().changeX(-delta);
    }

    // if on bottom wall
    if (line.x1() >= 0 && line.x1() <= WIDTH && line.y1() == 0)
    {
      line.rp1().changeX(delta);
    }

    // if on left wall
    if (line.x1() == 0 && line.y1() >= 0 && line.y1() <= HEIGHT)
    {
      line.rp1().changeY(-delta);
    }

    // if on right wall
    if (line.x1() == WIDTH && line.y1() >= 0 && line.y1() <= HEIGHT)
    {
      line.rp1().changeY(delta);
    }

    lineVector.set(line.dx(), line.dy());
  }

  if (keyStates['d'])
  {
    // if on top wall
    if (line.x1() >= 0 && line.x1() <= WIDTH && line.y1() == HEIGHT)
    {
      line.rp1().changeX(delta);
    }

    // if on bottom wall
    if (line.x1() >= 0 && line.x1() <= WIDTH && line.y1() == 0)
    {
      line.rp1().changeX(-delta);
    }

    // if on left wall
    if (line.x1() == 0 && line.y1() >= 0 && line.y1() <= HEIGHT)
    {
      line.rp1().changeY(delta);
    }

    // if on right wall
    if (line.x1() == WIDTH && line.y1() >= 0 && line.y1() <= HEIGHT)
    {
      line.rp1().changeY(-delta);
    }

    lineVector.set(line.dx(), line.dy());
  }

  if (keyStates['4'])
  {
    // if on top wall
    if (line.x2() >= 0 && line.x2() <= WIDTH && line.y2() == HEIGHT)
    {
      line.rp2().changeX(delta);
    }

    // if on bottom wall
    if (line.x2() >= 0 && line.x2() <= WIDTH && line.y2() == 0)
    {
      line.rp2().changeX(-delta);
    }

    // if on left wall
    if (line.x2() == 0 && line.y2() >= 0 && line.y2() <= HEIGHT)
    {
      line.rp2().changeY(delta);
    }

    // if on right wall
    if (line.x2() == WIDTH && line.y2() >= 0 && line.y2() <= HEIGHT)
    {
      line.rp2().changeY(-delta);
    }

    lineVector.set(line.dx(), line.dy());
  }

  if (keyStates['6'])
  {
    // if on top wall
    if (line.x2() >= 0 && line.x2() <= WIDTH && line.y2() == HEIGHT)
    {
      line.rp2().changeX(-delta);
    }

    // if on bottom wall
    if (line.x2() >= 0 && line.x2() <= WIDTH && line.y2() == 0)
    {
      line.rp2().changeX(delta);
    }

    // if on left wall
    if (line.x2() == 0 && line.y2() >= 0 && line.y2() <= HEIGHT)
    {
      line.rp2().changeY(-delta);
    }

    // if on right wall
    if (line.x2() == WIDTH && line.y2() >= 0 && line.y2() <= HEIGHT)
    {
      line.rp2().changeY(delta);
    }

    lineVector.set(line.dx(), line.dy());
  }
}

void checkGameStatus()
{
  if (ball1.getPoints() < 3 || ball2.getPoints() > 19)
  {
    std::cout << "Red player wins!" << std::endl;
    glutLeaveMainLoop();
  }

  if (ball2.getPoints() < 3 || ball1.getPoints() > 19)
  {
    std::cout << "Blue player wins!" << std::endl;
    glutLeaveMainLoop();
  }
}

// Collision detection --------------------------------------------------------
void detectLineCollision()
{
  if (c1->distanceToLineSquared(line) <= ballSizeSquared)
    vec1.reflectFrom(lineVector);

  if (c2->distanceToLineSquared(line) <= ballSizeSquared)
    vec2.reflectFrom(lineVector);
}

void detectWallCollision()
{
  if (c1->distanceToLineSquared(leftWall) <= ballSizeSquared)
    vec1.reflectFrom(leftWallVector);

  if (c1->distanceToLineSquared(rightWall) <= ballSizeSquared)
    vec1.reflectFrom(rightWallVector);

  if (c1->distanceToLineSquared(topWall) <= ballSizeSquared)
    vec1.reflectFrom(topWallVector);

  if (c1->distanceToLineSquared(bottomWall) <= ballSizeSquared)
    vec1.reflectFrom(bottomWallVector);

  if (c2->distanceToLineSquared(leftWall) <= ballSizeSquared)
    vec2.reflectFrom(leftWallVector);

  if (c2->distanceToLineSquared(rightWall) <= ballSizeSquared)
    vec2.reflectFrom(rightWallVector);

  if (c2->distanceToLineSquared(topWall) <= ballSizeSquared)
    vec2.reflectFrom(topWallVector);

  if (c2->distanceToLineSquared(bottomWall) <= ballSizeSquared)
    vec2.reflectFrom(bottomWallVector);
}

void detectFoodCollision()
{
  if (Point2D::distance2(*c1, food1) < ballSizeSquared)
  {
    food1.setX(disX(gen));
    food1.setY(disY(gen));
    ball1++;
    checkGameStatus();
  }

  if (Point2D::distance2(*c1, food2) < ballSizeSquared)
  {
    food2.setX(disX(gen));
    food2.setY(disY(gen));
    ball1--;
    checkGameStatus();
  }

  if (Point2D::distance2(*c2, food1) < ballSizeSquared)
  {
    food1.setX(disX(gen));
    food1.setY(disY(gen));
    ball2--;
    checkGameStatus();
  }

  if (Point2D::distance2(*c2, food2) < ballSizeSquared)
  {
    food2.setX(disX(gen));
    food2.setY(disY(gen));
    ball2++;
    checkGameStatus();
  }
}

// Display function -----------------------------------------------------------
void display()
{
  keyOperations();
  glClear(GL_COLOR_BUFFER_BIT);

  line.draw();
  ball1.drawDiagonals();
  ball1Outer.draw();
  ball2.drawDiagonals();
  ball2Outer.draw();
  food1.draw();
  food2.draw();

  glutSwapBuffers();

  ball1.translate(vec1);
  ball2.translate(vec2);
  ball1Outer.translate(vec1);
  ball2Outer.translate(vec2);

  detectLineCollision();
  detectWallCollision();
  detectFoodCollision();
}

void gameUpdate(int n)
{
  glutPostRedisplay();
  glutTimerFunc(refreshRate, gameUpdate, 0);
}

// Main function --------------------------------------------------------------
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                GLUT_ACTION_GLUTMAINLOOP_RETURNS);
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
