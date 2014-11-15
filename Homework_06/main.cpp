#include <GL/freeglut.h>
#include <iostream>
#include "Polygon2D.h"
#include "Line.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Point2DH<GLdouble> Point2DH;
typedef Utils::Polygon2D<GLdouble> Polygon2D;
typedef Utils::Line<GLdouble> Line;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color pColor(Utils::RED);
const Utils::Color ipColor(Utils::MAGENTA);
const Utils::Color spColor(Utils::GREEN);
const Utils::Color lColor(Utils::BLACK);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

Polygon2D poly;
Point2D *clicked = nullptr;
Point2D *rightClicked = nullptr;

void init()
{
  bgColor.setGLClearColor();

  // Display settings
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPointSize(10);
  glLineWidth(2);

  poly.addPoint(100, HEIGHT - 100);
  poly.addPoint(WIDTH - 100, HEIGHT - 100);
  poly.addPoint(WIDTH - 100, 100);
  poly.addPoint(500, 50);
  poly.addPoint(100, 100);
  poly.lineWidth = 2;
  poly.pointSize = 10;
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  poly.draw();
  poly.drawPoints();

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    clicked = poly.checkClick(xMouse, HEIGHT - yMouse, 12);
  }

  if(button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
  {
    rightClicked = poly.checkClick(xMouse, HEIGHT - yMouse, 12);
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    if(clicked)
    {
      clicked = nullptr;
      poly.release();
    }
  }

  if(button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
  {
    if(rightClicked)
    {
      rightClicked = nullptr;
      poly.release();
    }
  }

}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if(clicked)
  {
    poly.handleClick(xMouse, HEIGHT - yMouse, clicked);
  }

  if(rightClicked)
  {
    poly.handleRightClick(xMouse, HEIGHT - yMouse, rightClicked);
  }

  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 06");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
