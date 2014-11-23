#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Polygon2D.h"
#include "Line.h"
#include "Bezier2D.h"
#include "Slider.h"
#include "Vector2D.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Bezier2D<GLdouble> Bezier2D;
typedef Utils::Vector2D<GLdouble> Vector2D;
typedef Utils::Slider Slider;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;
const int gridSize = 40;

// Active points --------------------------------------------------------------
Point2D *clicked = nullptr;

// Info text ------------------------------------------------------------------
std::string tText;
std::stringstream ss;

Bezier2D b1;
Bezier2D b2;

Slider slider(100, 40, WIDTH - 100, 40);

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
}

void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "t: " << static_cast<double>(slider.getValue()) / 100 << std::endl;
  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

void drawGrid(int width, int height, int gap, GLfloat lineWidth,
              const Utils::Color& color)
{
  color.setGLColor();
  glLineWidth(lineWidth);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xAAAA);
  glBegin(GL_LINES);

  for(int i = gap; i < width; i += gap)
  {
    glVertex2i(i, 0);
    glVertex2i(i, height);
  }

  for(int i = gap; i < height; i += gap)
  {
    glVertex2i(0, i);
    glVertex2i(width, i);
  }

  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // draw grid to into background
  drawGrid(WIDTH, HEIGHT, gridSize, lineWidth, Utils::VERY_LIGHT_GRAY);

  // draw info text
  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  b1.drawControlPolygon();
  b1.drawBernstein();
  b1.drawPoints();

  b2.drawControlPolygon();
  b2.draw();
  b2.drawInterPolations(static_cast<double>(slider.getValue()) / 100.0f);
  b2.drawPoints();

  slider.draw();

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    clicked = b1.checkClick(xMouse, HEIGHT - yMouse, 12);

    if (!clicked)
      clicked = b2.checkClick(xMouse, HEIGHT - yMouse, 12);

    if (!clicked)
    {
      slider.checkClick(xMouse, HEIGHT - yMouse, 12);
      if (!slider.isDragging())
      {
        if (b1.getPoints() < 2)
        {
          b1.addPoint(xMouse, HEIGHT - yMouse);
        }
        else if (b1.getPoints() < 5)
        {
          b1.addPoint(xMouse, HEIGHT - yMouse);

          Vector2D q(b1.controlPoints.at(1), b1.controlPoints.front());

          b1.addPoint(b1.controlPoints.front().translated(q));
          b1.controlPoints.back().color = Utils::LIGHT_GRAY;
          b1.controlPoints.back().disabled = true;

          b1.addPoint(b1.controlPoints.front().x(),
                      b1.controlPoints.front().y());
          b1.controlPoints.back().disabled = true;
        }
        else if (b2.getPoints() < 1)
        {
          b2.addPoint(xMouse, HEIGHT - yMouse);
        }
        else if (b2.getPoints() < 6)
        {
          auto r2 = b2.controlPoints.back();
          Point2D r3(xMouse, HEIGHT - yMouse);
          b2.controlPoints.pop_back();

          b2.addPoint(b1.controlPoints.front());
          b2.controlPoints.back().disabled = true;
          
          Vector2D q(b1.controlPoints.front(), b1.controlPoints.at(1));

          b2.addPoint(b1.controlPoints.front().translated(0.8 * q));
          b2.controlPoints.back().color = Utils::LIGHT_GRAY;
          b2.controlPoints.back().disabled = true;

          b2.addPoint(r2);
          b2.addPoint(r3);

          b2.addPoint(b2.controlPoints.front().translated(0.8 * -q));
          b2.controlPoints.back().color = Utils::LIGHT_GRAY;
          b2.controlPoints.back().disabled = true;

          b2.addPoint(b1.controlPoints.front());
          b2.controlPoints.back().disabled = true;
        }
      }
    }
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    if (slider.isDragging())
      slider.release();

    if (clicked)
    {
      clicked->clicked = false;
      clicked = nullptr;

      if (b1.clicked)
        b1.release();

      if (b2.clicked)
        b1.release();
    }
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (clicked)
  {
    if (b1.clicked)
    {
      b1.handleClick(xMouse, HEIGHT - yMouse, clicked);

      Vector2D q(b1.controlPoints.at(1), b1.controlPoints.front());

      b1.controlPoints.at(3).setXY(b1.controlPoints.at(1).translated(2 * q));
      b1.controlPoints.back().setXY(b1.controlPoints.front());

      if (b2.getPoints())
      {
        b2.controlPoints.at(1).setXY(b1.controlPoints.at(1).translated(0.2 * q));
        b2.controlPoints.front().setXY(b1.controlPoints.front());
        b2.controlPoints.at(4).setXY(b1.controlPoints.at(4).translated(0.8 * q));
        b2.controlPoints.back().setXY(b1.controlPoints.front());
      }
    }

    if (b2.clicked)
      b2.handleClick(xMouse, HEIGHT - yMouse, clicked);

    glutPostRedisplay();
  }

  if (slider.isDragging())
  {
    slider.setHandlePos(xMouse);
    glutPostRedisplay();
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 07");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
