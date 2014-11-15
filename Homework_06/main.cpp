#include <GL/freeglut.h>
#include <iostream>
#include "Polygon2D.h"
#include "Line.h"
#include "Circle.h"
#include "PolyStar.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Point2DH<GLdouble> Point2DH;
typedef Utils::Polygon2D<GLdouble> Polygon2D;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Circle<GLdouble> Circle;
typedef Utils::PolyStar<GLdouble> PolyStar;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color fadedColor(Utils::VERY_LIGHT_GRAY);
const Utils::Color mountainColor("#ADDFFF");
const Utils::Color bushColor(Utils::DARK_GREEN);
const Utils::Color treeColor("#966F33");
const Utils::Color sunColor(Utils::ORANGE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

// Scene ----------------------------------------------------------------------
Circle glass1(400, 300, 60, 4);
Circle glass2(220, 300, 60, 5);

Polygon2D mountain;
Polygon2D bush;
Polygon2D tree;
Polygon2D treeBush;

Circle sun1(200, HEIGHT - 150, 60, 3);
Circle sun2(200, HEIGHT - 150, 60, 3);

Point2D *clicked = nullptr;
Point2D *rightClicked = nullptr;
std::vector<Polygon2D> polyVector;
std::vector<Polygon2D> glassesVector;

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

  mountain.addPoint(1020, 100);
  mountain.addPoint(840, 560);
  mountain.addPoint(560, 270);
  mountain.addPoint(300, 380);
  mountain.addPoint(200, 100);
  mountain.color = mountainColor;
  mountain.lineWidth = lineWidth;
  polyVector.push_back(mountain);

  bush.addPoint(700, 100);
  bush.addPoint(740, 160);
  bush.addPoint(640, 240);
  bush.addPoint(540, 170);
  bush.addPoint(580, 100);
  bush.color = bushColor;
  bush.lineWidth = lineWidth;
  polyVector.push_back(bush);

  tree.addPoint(880, 100);
  tree.addPoint(880, 380);
  tree.addPoint(820, 380);
  tree.addPoint(820, 100);
  tree.color = treeColor;
  tree.lineWidth = lineWidth;
  polyVector.push_back(tree);

  treeBush.addPoint(990, 500);
  treeBush.addPoint(850, 600);
  treeBush.addPoint(720, 540);
  treeBush.addPoint(750, 440);
  treeBush.addPoint(820, 380);
  treeBush.addPoint(880, 380);
  treeBush.color = bushColor;
  treeBush.lineWidth = lineWidth;
  polyVector.push_back(treeBush);

  sun1.color = sunColor;
  sun1.lineWidth = lineWidth;
  polyVector.push_back(sun1.toPolygon2D());

  sun2.color = sunColor;
  sun2.lineWidth = lineWidth;
  sun2.rotate(2 * Utils::PI / 6);
  polyVector.push_back(sun2.toPolygon2D());

  glass1.lineWidth = lineWidth;
  glassesVector.push_back(glass1.toPolygon2D());

  glass2.lineWidth = lineWidth;
  glassesVector.push_back(glass2.toPolygon2D());
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  for(const auto& p : polyVector)
    p.drawWithOtherColor(fadedColor);

  for(const auto& p : polyVector)
  {
    for(const auto& glass : glassesVector)
    {
      Polygon2D clipped = p.clipWith(glass);
      clipped.filled = true;
      clipped.color = p.color;
      clipped.draw();
    }
  }

  for(const auto& g : glassesVector)
    g.draw();

  glBegin(GL_LINES);
  Utils::glVertex2<GLdouble>(glassesVector[0].pointsContainer[2]);
  Utils::glVertex2<GLdouble>(glassesVector[1].pointsContainer[0]);
  glEnd();

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    for(auto& p : glassesVector)
    {
      if(clicked = p.checkClick(xMouse, HEIGHT - yMouse, 12))
        break;
    }
  }

  if(button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
  {
    for(auto& p : glassesVector)
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
      for(auto& p : glassesVector)
      {
        if(p.clicked)
        {
          p.release();
          break;
        }
      }
      glutPostRedisplay();
    }
  }

  if(button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
  {
    if(rightClicked)
    {
      rightClicked = nullptr;
      for(auto& p : glassesVector)
      {
        if(p.clicked)
        {
          p.release();
          break;
        }
      }
      glutPostRedisplay();
    }
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if(clicked)
  {
    for(auto& p : glassesVector)
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
    for(auto& poly : glassesVector)
    {
      for(auto& point : poly.pointsContainer)
      {
        if(rightClicked == &point)
          continue;

        auto dx = point.x() - rightClicked->x();
        auto dy = point.y() - rightClicked->y();
        point.setXY(xMouse + dx, HEIGHT - yMouse + dy);
      }
    }

    rightClicked->setXY(xMouse, HEIGHT - yMouse);
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
