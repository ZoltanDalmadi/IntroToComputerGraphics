#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Rectangle.h"
#include "Matrix.h"
#include "Point2D.h"
#include "Point3D.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------
typedef Utils::Rectangle<GLdouble> Rect;
typedef Utils::CavalierProjection<GLdouble> CvP;
typedef Utils::WindowToViewport<GLdouble> WTV;
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Point3DH<GLdouble> Point3DH;

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
const int gridSize = 40;

// ----------------------------------------------------------------------------
// Miscellaneous variables
// ----------------------------------------------------------------------------
double step = 0.5f;
double p = 0.0f;
double xMin = -20.0f;
double xMax = 20.0f;
double yMin = -20.0f;
double yMax = 20.0f;
double projAngle = 235.0;

// ----------------------------------------------------------------------------
// Window and viewport
// ----------------------------------------------------------------------------
Rect window(xMin, yMin, xMax, yMax);
Rect viewport(280, 0, 280 + HEIGHT, HEIGHT);

// ----------------------------------------------------------------------------
// Matrices
// ----------------------------------------------------------------------------
WTV wtv1(window, viewport);
CvP cvp(Utils::degToRad(projAngle));
auto T = wtv1 * cvp;

// ----------------------------------------------------------------------------
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;

// ----------------------------------------------------------------------------
// Math function to plot
// ----------------------------------------------------------------------------
Point3DH f(double x, double y, double p)
{
  return Point3DH(x, y, std::sin(std::sqrt(x * x + y * y) + p));
}

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
}

// ----------------------------------------------------------------------------
// Info test function. Shows current value of projection angle
// ----------------------------------------------------------------------------
void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "Projection angle: " << projAngle << " degrees" << std::endl;
  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

// ----------------------------------------------------------------------------
// Draw grid floor
// ----------------------------------------------------------------------------
void drawGrid(double start, double end, double gap, GLfloat lineWidth,
              const Utils::Color& color, const Utils::Matrix<GLdouble>& mat)
{
  color.setGLColor();
  glLineWidth(lineWidth);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xAAAA);
  glBegin(GL_LINES);

  for (double i = start; i <= end; i += gap)
  {
    Utils::Point3DH<GLdouble> asd1(i, end, -2);
    Utils::glVertex2<GLdouble>(asd1.transformed(mat).normalized2D());
    Utils::Point3DH<GLdouble> asd2(i, start, -2);
    Utils::glVertex2<GLdouble>(asd2.transformed(mat).normalized2D());
  }

  for (double i = start; i <= end; i += gap)
  {
    Utils::Point3DH<GLdouble> asd1(start, i, -2);
    Utils::glVertex2<GLdouble>(asd1.transformed(mat).normalized2D());
    Utils::Point3DH<GLdouble> asd2(end, i, -2);
    Utils::glVertex2<GLdouble>(asd2.transformed(mat).normalized2D());
  }

  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

// ----------------------------------------------------------------------------
// Main display function
// ----------------------------------------------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  drawGrid(xMin - 2, xMax + 2, 1.0, 1.0, Utils::VERY_LIGHT_GRAY, T);

  for (double x = xMin; x < xMax; x += step)
  {
    for (double y = yMin; y < yMax; y += step)
    {
      auto f1 = f(x, y, p).transformed(T).normalized2D();
      auto f2 = f(x + step, y, p).transformed(T).normalized2D();
      auto f3 = f(x + step, y + step, p).transformed(T).normalized2D();
      auto f4 = f(x, y + step, p).transformed(T).normalized2D();
      //graphColor.setGLColor();
      glColor3f(static_cast<GLfloat>((std::abs(x) + 20) / 40),
                static_cast<GLfloat>((std::abs(y) + 20) / 40), 0.4f);

      glBegin(GL_POLYGON);
      Utils::glVertex2<GLdouble>(f1);
      Utils::glVertex2<GLdouble>(f2);
      Utils::glVertex2<GLdouble>(f3);
      Utils::glVertex2<GLdouble>(f4);
      glEnd();

      graphGridColor.setGLColor();
      glBegin(GL_LINE_LOOP);
      Utils::glVertex2<GLdouble>(f1);
      Utils::glVertex2<GLdouble>(f2);
      Utils::glVertex2<GLdouble>(f3);
      Utils::glVertex2<GLdouble>(f4);
      glEnd();
    }
  }

  glutSwapBuffers();
}

void keyPressed(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
  {
    if (projAngle < 360)
    {
      projAngle++;
      cvp.setAlpha(Utils::degToRad(projAngle));
      T = wtv1 * cvp;
    }
  }

  if (key == GLUT_KEY_DOWN)
  {
    if (projAngle > 180)
    {
      projAngle--;
      cvp.setAlpha(Utils::degToRad(projAngle));
      T = wtv1 * cvp;
    }
  }
}

void appUpdate(int n)
{
  p -= 0.1;
  glutPostRedisplay();
  glutTimerFunc(refreshRate, appUpdate, 0);
}

// ----------------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Homework 09");

  init();
  glutDisplayFunc(display);
  glutSpecialFunc(keyPressed);
  glutTimerFunc(refreshRate, appUpdate, 0);
  glutMainLoop();
  return 0;
}
