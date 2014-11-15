#include <GL/freeglut.h>
#include <iostream>
#include <vector>
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
Polygon2D clipper;
Point2D *clicked = nullptr;
Point2D *rightClicked = nullptr;
std::vector<Polygon2D> polyVector;

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

  poly.addPoint(WIDTH - 400, HEIGHT - 200);
  poly.addPoint(400, HEIGHT - 200);
  poly.addPoint(400, 200);
  poly.addPoint(WIDTH - 400, 200);
  poly.addPoint(600, 300);
  poly.lineWidth = 2;
  poly.pointSize = 10;

  clipper.addPoint(WIDTH - 200, HEIGHT - 300);
  clipper.addPoint(200, HEIGHT - 100);
  clipper.addPoint(300, 100);
  clipper.addPoint(WIDTH - 200, 300);
  clipper.lineWidth = 2;
  clipper.pointSize = 10;
  clipper.pointColor = Utils::BLUE;
  clipper.color = ipColor;

  polyVector.push_back(poly);
  polyVector.push_back(clipper);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  Polygon2D asd = polyVector[0].clipWith(polyVector[1]);
  asd.filled = true;
  asd.color = Utils::RED;
  asd.draw();

  for(auto& p : polyVector)
  {
    p.draw();
    p.drawPoints();
  }

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    for(auto& p : polyVector)
    {
      if(clicked = p.checkClick(xMouse, HEIGHT - yMouse, 12))
        break;
    }
  }

  if(button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
  {
    for(auto& p : polyVector)
    {
      if(rightClicked = p.checkClick(xMouse, HEIGHT - yMouse, 12))
        break;
    }
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    if(clicked)
    {
      clicked = nullptr;
      for(auto& p : polyVector)
      {
        if(p.clicked)
        {
          p.release();
          break;
        }
      }
    }
  }

  if(button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
  {
    if(rightClicked)
    {
      rightClicked = nullptr;
      for(auto& p : polyVector)
      {
        if(p.clicked)
        {
          p.release();
          break;
        }
      }
    }
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if(clicked)
  {
    for(auto& p : polyVector)
    {
      if(p.clicked)
      {
        p.handleClick(xMouse, HEIGHT - yMouse, clicked);
        break;
      }
    }
  }

  if(rightClicked)
  {
    for(auto& p : polyVector)
    {
      if(p.clicked)
      {
        p.handleRightClick(xMouse, HEIGHT - yMouse, rightClicked);
        break;
      }
    }
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
