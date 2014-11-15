#include <GL/freeglut.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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
const Utils::Color pColor(Utils::RED);
const Utils::Color ipColor(Utils::MAGENTA);
const Utils::Color spColor(Utils::GREEN);
const Utils::Color lColor(Utils::BLACK);
const Utils::Color sunColor(Utils::ORANGE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

// Info text ------------------------------------------------------------------
std::string tText;
std::stringstream ss;


// Scene ----------------------------------------------------------------------
Circle glasses(400, 300, 200, 6);
auto glasses_poly = glasses.toPolygon2D();
Circle sun(200, HEIGHT - 150, 60, 12);

Polygon2D mountain;
Polygon2D bush;
Polygon2D tree;
Polygon2D treeBush;



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

  mountain.addPoint(1020, 100);
  mountain.addPoint(840, 560);
  mountain.addPoint(560, 270);
  mountain.addPoint(300, 380);
  mountain.addPoint(200, 100);
  mountain.color = Utils::LIGHT_GRAY;
  mountain.lineWidth = lineWidth;
  polyVector.push_back(mountain);

  bush.addPoint(700, 100);
  bush.addPoint(740, 160);
  bush.addPoint(640, 240);
  bush.addPoint(540, 170);
  bush.addPoint(580, 100);
  bush.color = Utils::LIGHT_GRAY;
  bush.lineWidth = lineWidth;
  polyVector.push_back(bush);

  tree.addPoint(880, 100);
  tree.addPoint(880, 380);
  tree.addPoint(820, 380);
  tree.addPoint(820, 100);
  tree.color = Utils::LIGHT_GRAY;
  tree.lineWidth = lineWidth;
  polyVector.push_back(tree);

  treeBush.addPoint(990, 500);
  treeBush.addPoint(850, 600);
  treeBush.addPoint(720, 540);
  treeBush.addPoint(750, 440);
  treeBush.addPoint(820, 380);
  treeBush.addPoint(880, 380);
  treeBush.color = Utils::LIGHT_GRAY;
  treeBush.lineWidth = lineWidth;
  polyVector.push_back(treeBush);

  sun.color = sunColor;
  sun.filled = true;
  polyVector.push_back(sun.toPolygon2D());
}

void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  if(clicked)
    ss << "Active point's coordinates: " << clicked->x() << ", " << clicked->y() << std::endl;
  else
    ss << "No active point selected" << std::endl;
  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  //Polygon2D asd = polyVector[0].clipWith(polyVector[1]);

  //asd.filled = true;
  //asd.color = Utils::RED;
  //asd.draw();

  for(auto& p : polyVector)
  {
    p.draw();
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
      glutPostRedisplay();
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
      glutPostRedisplay();
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
