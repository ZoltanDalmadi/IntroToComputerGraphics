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
const Utils::Color glassesColor("#00000014");

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

// Active points --------------------------------------------------------------
Point2D *clicked = nullptr;
Point2D *rightClicked = nullptr;

// Object containers ----------------------------------------------------------
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

  // Assemble scene -----------------------------------------------------------
  // mountain
  polyVector.emplace_back();
  polyVector[0].addPoint(1020, 100);
  polyVector[0].addPoint(840, 560);
  polyVector[0].addPoint(560, 270);
  polyVector[0].addPoint(300, 380);
  polyVector[0].addPoint(200, 100);
  polyVector[0].color = mountainColor;
  polyVector[0].lineWidth = lineWidth;

  // bush
  polyVector.emplace_back();
  polyVector[1].addPoint(700, 100);
  polyVector[1].addPoint(740, 160);
  polyVector[1].addPoint(640, 240);
  polyVector[1].addPoint(540, 170);
  polyVector[1].addPoint(580, 100);
  polyVector[1].color = bushColor;
  polyVector[1].lineWidth = lineWidth;

  // tree
  polyVector.emplace_back();
  polyVector[2].addPoint(880, 100);
  polyVector[2].addPoint(880, 380);
  polyVector[2].addPoint(820, 380);
  polyVector[2].addPoint(820, 100);
  polyVector[2].color = treeColor;
  polyVector[2].lineWidth = lineWidth;

  // tree bush
  polyVector.emplace_back();
  polyVector[3].addPoint(990, 500);
  polyVector[3].addPoint(850, 600);
  polyVector[3].addPoint(720, 540);
  polyVector[3].addPoint(750, 440);
  polyVector[3].addPoint(820, 380);
  polyVector[3].addPoint(880, 380);
  polyVector[3].color = bushColor;
  polyVector[3].lineWidth = lineWidth;

  // sun1
  Circle sun1(200, HEIGHT - 150, 60, 3);
  polyVector.emplace_back(sun1.toPolygon2D());
  polyVector[4].color = sunColor;
  polyVector[4].lineWidth = lineWidth;

  // sun2
  Circle sun2(200, HEIGHT - 150, 60, 3);
  sun2.rotate(2 * Utils::PI / 6);
  polyVector.emplace_back(sun2.toPolygon2D());
  polyVector[5].color = sunColor;
  polyVector[5].lineWidth = lineWidth;

  // glasses
  Circle glass1(400, 300, 60, 4);
  glassesVector.emplace_back(glass1.toPolygon2D());
  glassesVector[0].lineWidth = lineWidth;

  Circle glass2(220, 300, 60, 5);
  glassesVector.push_back(glass2.toPolygon2D());
  glassesVector[1].lineWidth = lineWidth;

  // --------------------------------------------------------------------------
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xFF00);

  // draw background items
  for (const auto& p : polyVector)
    p.drawWithOtherColor(fadedColor);

  glDisable(GL_LINE_STIPPLE);

  // draw clipped polygons in glasses
  for (const auto& p : polyVector)
  {
    for (const auto& glass : glassesVector)
    {
      Polygon2D clipped = p.clipWith(glass);
      clipped.filled = true;
      clipped.color = p.color;
      clipped.draw();

      // draw outline
      clipped.filled = false;
      clipped.lineWidth = lineWidth;
      clipped.draw();
    }
  }

  // draw glasses
  for (auto& g : glassesVector)
  {
    g.color = glassesColor;
    g.filled = true;
    g.draw();

    // draw outline
    g.filled = false;
    g.color = Utils::BLACK;
    g.draw();
  }

  // draw line between the 2 glass
  glBegin(GL_LINES);
  Utils::glVertex2<GLdouble>(glassesVector[0].pointsContainer[2]);
  Utils::glVertex2<GLdouble>(glassesVector[1].pointsContainer[0]);
  glEnd();

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    for (auto& p : glassesVector)
    {
      if ((clicked = p.checkClick(xMouse, HEIGHT - yMouse, 12)))
        break;
    }
  }

  if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
  {
    for (auto& p : glassesVector)
    {
      if ((rightClicked = p.checkClick(xMouse, HEIGHT - yMouse, 12)))
        break;
    }
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    if (clicked)
    {
      clicked = nullptr;

      for (auto& p : glassesVector)
      {
        if (p.clicked)
        {
          p.release();
          break;
        }
      }
    }
  }

  if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
  {
    if (rightClicked)
    {
      rightClicked = nullptr;

      for (auto& p : glassesVector)
      {
        if (p.clicked)
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
  if (clicked)
  {
    for (auto& p : glassesVector)
    {
      if (p.clicked)
      {
        p.handleClick(xMouse, HEIGHT - yMouse, clicked);
        break;
      }
    }
  }

  // move both glasses on right click
  if (rightClicked)
  {
    for (auto& poly : glassesVector)
    {
      for (auto& point : poly.pointsContainer)
      {
        if (rightClicked == &point)
          continue;

        // cache difference vector between current and dragged point
        auto dx = point.x() - rightClicked->x();
        auto dy = point.y() - rightClicked->y();

        // translate current point with difference vector
        point.setXY(xMouse + dx, HEIGHT - yMouse + dy);
      }
    }

    // translate dragged point itself
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
