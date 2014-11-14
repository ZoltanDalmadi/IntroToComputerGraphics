#include <GL/freeglut.h>
#include <iostream>
#include <sstream>
#include <string>
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

// Info text ------------------------------------------------------------------
std::string tText;
std::stringstream ss;

Point2D P1(200, 300);
Point2D P2(600, 500);

Point2D P3(500, 400);
Point2D P4(800, 500);

Line line1(100, 150, 400, 400);
Line line2(80, 50, 300, 100);

Point2D P5(300, 100);

Point2D intersection;
Point2D intersection2;
Polygon2D poly;

// intersection point
Point2DH crossProduct(const Point2DH& u, const Point2DH& v)
{
  return Point2DH(u.y() * v.w() - v.y() * u.w(),
                  -(u.x() * v.w() - v.x() * u.w()),
                  u.x() * v.y() - v.x() * u.y());
}

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

  poly.addPoint(250, 300);
  poly.addPoint(650, 500);
  poly.addPoint(300, 200);
}

void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "Intersection point: (" << intersection.x() << ", " <<
                                   intersection.y() << ")" << std::endl;

  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  lColor.setGLColor();
  glBegin(GL_LINES);
  glVertex2d(P1.x(), P1.y());
  glVertex2d(P2.x(), P2.y());
  glEnd();

  pColor.setGLColor();
  glBegin(GL_POINTS);
  glVertex2d(P1.x(), P1.y());
  glVertex2d(P2.x(), P2.y());
  glEnd();

  lColor.setGLColor();
  glBegin(GL_LINES);
  glVertex2d(P3.x(), P3.y());
  glVertex2d(P4.x(), P4.y());
  glEnd();

  pColor.setGLColor();
  glBegin(GL_POINTS);
  glVertex2d(P3.x(), P3.y());
  glVertex2d(P4.x(), P4.y());
  glEnd();

  // draw intersection line
  ipColor.setGLColor();
  intersection.draw();

  line1.draw();
  line1.drawPoints();
  line2.draw();
  line2.drawPoints();
  intersection2.draw();

  P5.draw();
  poly.draw();

  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    P1.checkClick(xMouse, HEIGHT - yMouse, 12);
    P2.checkClick(xMouse, HEIGHT - yMouse, 12);
    P3.checkClick(xMouse, HEIGHT - yMouse, 12);
    P4.checkClick(xMouse, HEIGHT - yMouse, 12);
    P5.checkClick(xMouse, HEIGHT - yMouse, 12);
    poly.checkClick(xMouse, HEIGHT - yMouse, 12);
    line1.rp1().checkClick(xMouse, HEIGHT - yMouse, 12);
    line1.rp2().checkClick(xMouse, HEIGHT - yMouse, 12);
    line2.rp1().checkClick(xMouse, HEIGHT - yMouse, 12);
    line2.rp2().checkClick(xMouse, HEIGHT - yMouse, 12);
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    P1.release();
    P2.release();
    P3.release();
    P4.release();
    P5.release();
    poly.release();
    line1.rp1().release();
    line1.rp2().release();
    line2.rp1().release();
    line2.rp2().release();
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if(P1.clicked)
  {
    P1.setX(xMouse);
    P1.setY(HEIGHT - yMouse);
    intersection = Line::computeIntersectionPoint(Line::getParams(P1, P2),
                                                  Line::getParams(P3, P4));
  }

  if(P2.clicked)
  {
    P2.setX(xMouse);
    P2.setY(HEIGHT - yMouse);
    intersection = Line::computeIntersectionPoint(Line::getParams(P1, P2),
                                                  Line::getParams(P3, P4));
  }

  if(P3.clicked)
  {
    P3.setX(xMouse);
    P3.setY(HEIGHT - yMouse);
    intersection = Line::computeIntersectionPoint(Line::getParams(P1, P2),
                                                  Line::getParams(P3, P4));
  }

  if(P4.clicked)
  {
    P4.setX(xMouse);
    P4.setY(HEIGHT - yMouse);
    intersection = Line::computeIntersectionPoint(Line::getParams(P1, P2),
                                                  Line::getParams(P3, P4));
  }

  if(P5.clicked)
  {
    P5.setX(xMouse);
    P5.setY(HEIGHT - yMouse);
  }

  poly.handleClick(xMouse, HEIGHT - yMouse);

  if(line1.rp1().clicked)
  {
    line1.rp1().setX(xMouse);
    line1.rp1().setY(HEIGHT - yMouse);
    intersection2 = line1.computeIntersectionPoint(line2);
  }

  if(line1.rp2().clicked)
  {
    line1.rp2().setX(xMouse);
    line1.rp2().setY(HEIGHT - yMouse);
    intersection2 = line1.computeIntersectionPoint(line2);
  }

  if(line2.rp1().clicked)
  {
    line2.rp1().setX(xMouse);
    line2.rp1().setY(HEIGHT - yMouse);
    intersection2 = line2.computeIntersectionPoint(line1);
  }

  if(line2.rp2().clicked)
  {
    line2.rp2().setX(xMouse);
    line2.rp2().setY(HEIGHT - yMouse);
    intersection2 = line2.computeIntersectionPoint(line1);
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
