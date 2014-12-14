#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Rectangle.h"
#include "Matrix.h"
#include "Point3D.h"
#include "Color.h"
#include "Vector2D.h"
#include "Sphere.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------
typedef Utils::Rectangle<GLdouble> Rect;
typedef Utils::CentralProjection<GLdouble> CentralProjection;
typedef Utils::WindowToViewport<GLdouble> WTV;
typedef Utils::Rotate3DX<GLdouble> Rotate3DX;
typedef Utils::Rotate3DY<GLdouble> Rotate3DY;
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Point3D<GLdouble> Point3D;
typedef Utils::Point3DH<GLdouble> Point3DH;
typedef Utils::Sphere<GLdouble> Sphere;

// ----------------------------------------------------------------------------
// Window size
// ----------------------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// ----------------------------------------------------------------------------
// Colors
// ----------------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color graphColor(Utils::ORANGE);
const Utils::Color graphGridColor(Utils::BLACK);

// ----------------------------------------------------------------------------
// Refresh rate
// ----------------------------------------------------------------------------
const size_t refreshRate = 10;

// ----------------------------------------------------------------------------
// Sizes
// ----------------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;

// ----------------------------------------------------------------------------
// Miscellaneous variables
// ----------------------------------------------------------------------------
bool drag = false;
bool drag2 = false;
double lastRotX = 0.0f;
double lastRotY = 0.0f;
double lastLightX = 0.0f;
double lastLightY = 0.0f;
GLint clickedX;
GLint clickedY;
GLint clicked2X;
GLint clicked2Y;

// ----------------------------------------------------------------------------
// Window and viewport
// ----------------------------------------------------------------------------
Rect window(-1.5, -1.5, 1.5, 1.5);
Rect viewport(280, 0, 280 + HEIGHT, HEIGHT);

// ----------------------------------------------------------------------------
// Matrices
// ----------------------------------------------------------------------------
CentralProjection cp(8.0f);
WTV wtv(window, viewport);
Rotate3DX rx(0);
Rotate3DY ry(0);
Point3DH centerofProjection(0, 0, 8.0f, 1);
Point3DH lightSource(2, 2, 8, 1);

// ----------------------------------------------------------------------------
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;

Sphere sphere;

// ----------------------------------------------------------------------------
// Init function
// ----------------------------------------------------------------------------
void init()
{
  bgColor.setGLClearColor();
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  sphere.pointSize = 6.0f;
}

// ----------------------------------------------------------------------------
// Info text function. Shows current value of projection angle and FPS
// ----------------------------------------------------------------------------
void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "Projection distance: " << cp.getDistanceToOrigin() << std::endl;
  ss << "Segments: " << sphere.getSegments() << std::endl;
  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

// ----------------------------------------------------------------------------
// Main display function
// ----------------------------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  auto projTrans = wtv * cp;
  auto rxry = rx * ry;

  sphere.drawFaces(projTrans, rxry, centerofProjection, lightSource);
  //sphere.drawEdges(T);
  //sphere.drawPoints(T);

  glutSwapBuffers();
}

// ----------------------------------------------------------------------------
// Main mouse button handler
// ----------------------------------------------------------------------------
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    drag = true;

    // cache clicked point position
    clickedX = xMouse;
    clickedY = HEIGHT - yMouse;
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    drag = false;

    // cache current rotation values
    lastRotX = rx.getAngle();
    lastRotY = ry.getAngle();

    glutPostRedisplay();
  }

  if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
  {
    drag2 = true;

    // cache clicked point position
    clicked2X = xMouse;
    clicked2Y = HEIGHT - yMouse;
  }

  if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
  {
    drag2 = false;

    // cache current rotation values
    lastLightX = lightSource.x();
    lastLightY = lightSource.y();

    glutPostRedisplay();
  }
}

// ----------------------------------------------------------------------------
// Main mouse movement handler
// ----------------------------------------------------------------------------
void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (drag)
  {
    // calculate vector between clicked and dragged point
    Utils::Vector2D<GLint> v(clickedX, clickedY, xMouse, HEIGHT - yMouse);

    // set rotation values
    rx.setAngle(lastRotX - Utils::degToRad(v.y()) * 0.25);
    ry.setAngle(lastRotY + Utils::degToRad(v.x()) * 0.25);
  }

  if (drag2)
  {
    Utils::Vector2D<GLint> v(clicked2X, clicked2Y, xMouse, HEIGHT - yMouse);
    lightSource.setX(lastLightX + v.x() * 0.25);
    lightSource.setY(lastLightY + v.y() * 0.25);
  }

  glutPostRedisplay();
}

void keyPressed(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
  {
    auto old = cp.getDistanceToOrigin();
    cp.setDistanceToOrigin(old - 0.1f);
    centerofProjection.setZ(old - 0.1f);
  }

  if (key == GLUT_KEY_DOWN)
  {
    auto old = cp.getDistanceToOrigin();
    cp.setDistanceToOrigin(old + 0.1f);
    centerofProjection.setZ(old + 0.1f);
  }

  glutPostRedisplay();
}

// ----------------------------------------------------------------------------
// Mouse wheel handler
// ----------------------------------------------------------------------------
void wheelFunc(int wheel, int direction, int x, int y)
{
  if (direction > 0)
    sphere++;
  else if (sphere.getSegments() > 2)
    sphere--;

  glutPostRedisplay();
}

// ----------------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 10");

  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutSpecialFunc(keyPressed);
  glutMouseWheelFunc(wheelFunc);
  glutMainLoop();
  return 0;
}
