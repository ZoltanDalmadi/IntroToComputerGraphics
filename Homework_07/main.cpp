#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Polygon2D.h"
#include "Line.h"
#include "Bezier2D.h"
#include "Slider.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Bezier2D<GLdouble> Bezier2D;
typedef Utils::Slider Slider;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

// Active points --------------------------------------------------------------
Point2D *clicked = nullptr;

// Info text ------------------------------------------------------------------
std::string tText;
std::stringstream ss;

Bezier2D b1;

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

  //b1.addPoint(200, 300);
  //b1.addPoint(300, 700);
  //b1.addPoint(600, 600);
  //b1.addPoint(800, 200);
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

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  b1.drawControlPolygon();
  b1.drawBernstein();
  //b1.draw();
  //b1.drawInterPolations(static_cast<double>(slider.getValue()) / 100.0f);
  b1.drawPoints();

  slider.draw();

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
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

        auto dx = b1.controlPoints.front().x() - b1.controlPoints.at(1).x();
        auto dy = b1.controlPoints.front().y() - b1.controlPoints.at(1).y();

        b1.addPoint(b1.controlPoints.front().x() + dx,
                    b1.controlPoints.front().y() + dy);
        b1.controlPoints.back().color = Utils::VERY_LIGHT_GRAY;
        b1.controlPoints.back().disabled = true;

        b1.addPoint(b1.controlPoints.front().x(),
                    b1.controlPoints.front().y());
        b1.controlPoints.back().disabled = true;

      }

      clicked = b1.checkClick(xMouse, HEIGHT - yMouse, 12);
      //if (!clicked)

    }
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    if (slider.isDragging())
      slider.release();

    if (clicked)
    {
      clicked = nullptr;

      if (b1.clicked)
      {
        b1.release();
      }
    }
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (clicked)
  {
    if (b1.clicked)
      b1.handleClick(xMouse, HEIGHT - yMouse, clicked);
  }

  if(slider.isDragging())
    slider.setHandlePos(xMouse);

  glutPostRedisplay();
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
