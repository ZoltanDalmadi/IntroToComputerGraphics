#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Line.h"
#include "Matrix.h"

// Typedefs -------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Line<GLdouble> Line;
typedef Utils::Matrix<4, 4, GLdouble> Matrix4x4;
typedef Utils::Matrix<4, 3, GLdouble> Matrix3x4;
typedef Utils::Matrix<1, 4, GLdouble> Vector4x1;
typedef Utils::Matrix<1, 3, GLdouble> Vector3x1;

// Window size ----------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// Colors ---------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color curveColor(Utils::BLUE);

// Points ---------------------------------------------------------------------
Point2D P1(100, 150);
Point2D P2(300, 550);
Point2D P3(600, 350);
Point2D P4(1000, 600);
Line tangent(P1, P2);

GLdouble Gdata[16] = { tangent.x1(), P3.x(), P4.x(), tangent.dx(),
                       tangent.y1(), P3.y(), P4.y(), tangent.dy(),
                       1, 1, 1, 1 };

GLdouble Mdata[16] = { -1, 0, 1, 3,
                        1, 0, 1, -2,
                       -1, 0, 1, 1,
                        1, 1, 1, 0 };

Matrix3x4 G(Gdata);
Matrix4x4 M(Mdata);
Matrix4x4 M_inv = std::move(M.inverse());
Vector4x1 T;
Vector3x1 temp;

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

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  tangent.draw();
  tangent.drawPoints();
  P3.draw();
  P4.draw();
  Matrix3x4 C = G * M_inv;
  curveColor.setGLColor();
  glBegin(GL_LINE_STRIP);

  for(GLdouble i = -1.0f; i <= 1.0f; i += 0.01f)
  {
    T(0, 0) = i*i*i;
    T(1, 0) = i*i;
    T(2, 0) = i;
    T(3, 0) = 1;
    temp = C * T;
    glVertex2d(temp(0, 0), temp(1, 0));
  }

  glEnd();
  glutSwapBuffers();
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    tangent.rp2().checkClick(xMouse, HEIGHT - yMouse, 12);
  }

  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    tangent.rp2().release();
  }
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (tangent.rp2().clicked) {
    tangent.rp2().setX(xMouse);
    tangent.rp2().setY(HEIGHT-yMouse);
    G(0, 0) = tangent.x1();
    G(1, 0) = tangent.y1();
    G(0, 3) = tangent.dx();
    G(1, 3) = tangent.dy();
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
