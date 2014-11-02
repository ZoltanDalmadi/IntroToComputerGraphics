#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Line.h"
#include "Matrix.h"
#include "Slider.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Matrix<4, 4, GLdouble> Matrix4x4;
typedef Utils::Matrix<4, 3, GLdouble> Matrix3x4;
typedef Utils::Matrix<1, 4, GLdouble> Vector4x1;
typedef Utils::Matrix<1, 3, GLdouble> Vector3x1;
typedef Utils::Slider Slider;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color curveColor(Utils::BLUE);

// Sizes ----------------------------------------------------------------------
const GLfloat lineWidth = 2.0;

// Points ---------------------------------------------------------------------
Line tangent(100, 150, 300, 550);
Point2D P3(600, 350);
Point2D P4(1000, 600);

GLdouble minParam = -2.0f;
GLdouble maxParam = 2.0f;
GLdouble t1 = -1.0f;
GLdouble t2 = 0.0f;
GLdouble t3 = 1.0f;

Matrix3x4 G;
Matrix4x4 preM;
Matrix4x4 M;
Vector4x1 T;
Vector3x1 temp;

Slider t1Slider(100, 40, WIDTH-100, 40);
Slider t2Slider(100, 40, WIDTH-100, 40);
Slider t3Slider(100, 40, WIDTH-100, 40);

Line sliderLine(100, 40, WIDTH-100, 40);
Point2D notch1(sliderLine.pointAt(0.25));
Point2D notch2(sliderLine.pointAt(0.5));
Point2D notch3(sliderLine.pointAt(0.75));

void updateGMatrix()
{
  G(0, 0) = tangent.x1();
  G(1, 0) = tangent.y1();
  G(0, 1) = P3.x();
  G(1, 1) = P3.y();
  G(0, 2) = P4.x();
  G(1, 2) = P4.y();
  G(0, 3) = tangent.dx();
  G(1, 3) = tangent.dy();
}

void updateMMatrix()
{
  preM(0, 0) = t1 * t1 * t1;
  preM(1, 0) = t1 * t1;
  preM(2, 0) = t1;

  preM(0, 1) = t2 * t2 * t2;
  preM(1, 1) = t2 * t2;
  preM(2, 1) = t2;

  preM(0, 2) = t3 * t3 * t3;
  preM(1, 2) = t3 * t3;
  preM(2, 2) = t3;

  // derivative for tangent
  preM(0, 3) = 3.0f * t1 * t1;
  preM(1, 3) = 2.0f * t1 ;

  // we need inverse of all this
  M = preM.inverse();
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
  tangent.lineWidth = lineWidth;

  // these parts of the matrices never change
  G(2, 0) = 1.0;
  G(2, 1) = 1.0;
  G(2, 2) = 1.0;
  G(2, 3) = 1.0;
  preM(3, 0) = 1.0;
  preM(3, 1) = 1.0;
  preM(3, 2) = 1.0;
  preM(2, 3) = 1.0;
  preM(3, 3) = 0.0;

  // update matrices
  updateGMatrix();
  updateMMatrix();

  t1Slider.setValue(25);
  t2Slider.setValue(50);
  t3Slider.setValue(75);
  
  sliderLine.pointsColor = Utils::BLACK;
  t1Slider.setHandleSize(6);
  t2Slider.setHandleSize(6);
  t3Slider.setHandleSize(6);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  Matrix3x4 C = G * M;
  curveColor.setGLColor();
  glLineWidth(lineWidth);
  glBegin(GL_LINE_STRIP);

  for(GLdouble t = -1.0f; t <= 1.0f; t += 0.01f)
  {
    T(0, 0) = t*t*t;
    T(1, 0) = t*t;
    T(2, 0) = t;
    T(3, 0) = 1;
    temp = C * T;
    glVertex2d(temp(0, 0), temp(1, 0));
  }

  glEnd();
  tangent.draw();
  tangent.drawPoints();
  P3.draw();
  P4.draw();

  sliderLine.draw();
  sliderLine.drawPoints();
  notch1.draw();
  notch2.draw();
  notch3.draw();
  t1Slider.drawHandle();
  t2Slider.drawHandle();
  t3Slider.drawHandle();

  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    tangent.rp1().checkClick(xMouse, HEIGHT - yMouse, 12);
    tangent.rp2().checkClick(xMouse, HEIGHT - yMouse, 12);
    P3.checkClick(xMouse, HEIGHT - yMouse, 12);
    P4.checkClick(xMouse, HEIGHT - yMouse, 12);
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    tangent.rp1().release();
    tangent.rp2().release();
    P3.release();
    P4.release();
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if(tangent.rp1().clicked)
  {
    GLdouble dx = tangent.dx();
    GLdouble dy = tangent.dy();
    tangent.rp1().setX(xMouse);
    tangent.rp1().setY(HEIGHT - yMouse);
    tangent.rp2().setX(xMouse + dx);
    tangent.rp2().setY(HEIGHT - yMouse + dy);
    G(0, 0) = tangent.x1();
    G(1, 0) = tangent.y1();
  }

  if(tangent.rp2().clicked)
  {
    tangent.rp2().setX(xMouse);
    tangent.rp2().setY(HEIGHT - yMouse);
    G(0, 0) = tangent.x1();
    G(1, 0) = tangent.y1();
    G(0, 3) = tangent.dx();
    G(1, 3) = tangent.dy();
  }

  if(P3.clicked)
  {
    P3.setX(xMouse);
    P3.setY(HEIGHT - yMouse);
    G(0, 1) = P3.x();
    G(1, 1) = P3.y();
  }

  if(P4.clicked)
  {
    P4.setX(xMouse);
    P4.setY(HEIGHT - yMouse);
    G(0, 2) = P4.x();
    G(1, 2) = P4.y();
  }

  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 04");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
