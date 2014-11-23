#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include "Bezier2D.h"
#include "Slider.h"
#include "Vector2D.h"

// ----------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------
typedef Utils::Point2D<GLdouble> Point2D;
typedef Utils::Bezier2D<GLdouble> Bezier2D;
typedef Utils::Vector2D<GLdouble> Vector2D;
typedef Utils::Slider Slider;

// ----------------------------------------------------------------------------
// Window size
// ----------------------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// ----------------------------------------------------------------------------
// Colors
// ----------------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);

// ----------------------------------------------------------------------------
// Sizes
// ----------------------------------------------------------------------------
const GLfloat lineWidth = 2.0f;
const int gridSize = 40;

// ----------------------------------------------------------------------------
// Active point
// ----------------------------------------------------------------------------
Point2D *clicked = nullptr;

// ----------------------------------------------------------------------------
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;

// ----------------------------------------------------------------------------
// Curves
// ----------------------------------------------------------------------------
Bezier2D b1;
Bezier2D b2;

// ----------------------------------------------------------------------------
// Slider
// ----------------------------------------------------------------------------
Slider slider(100, 40, WIDTH - 100, 40);

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
// Info test function. Shows current value of t
// ----------------------------------------------------------------------------
void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "t: " << static_cast<double>(slider.getValue()) / 100 << std::endl;
  tText = ss.str();
  ss.str("");

  color.setGLColor();
  glRasterPos2i(x, y);
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)tText.c_str());
}

// ----------------------------------------------------------------------------
// Draw grid to background
// ----------------------------------------------------------------------------
void drawGrid(int width, int height, int gap, GLfloat lineWidth,
              const Utils::Color& color)
{
  color.setGLColor();
  glLineWidth(lineWidth);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0xAAAA);
  glBegin(GL_LINES);

  for (int i = gap; i < width; i += gap)
  {
    glVertex2i(i, 0);
    glVertex2i(i, height);
  }

  for (int i = gap; i < height; i += gap)
  {
    glVertex2i(0, i);
    glVertex2i(width, i);
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

  // draw grid to into background
  drawGrid(WIDTH, HEIGHT, gridSize, lineWidth, Utils::VERY_LIGHT_GRAY);

  // draw info text
  drawInfoText(10, HEIGHT - 24, Utils::BLACK);

  // draw first curve
  b1.drawControlPolygon();
  b1.drawBernstein();
  b1.drawPoints();

  // draw second curve
  b2.drawControlPolygon();
  b2.draw();
  b2.drawInterPolations(static_cast<double>(slider.getValue()) / 100.0f);
  b2.drawPoints();

  // draw slider
  slider.draw();

  glutSwapBuffers();
}

// ----------------------------------------------------------------------------
// Main mouse button handler
// ----------------------------------------------------------------------------
void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
  if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
  {
    // check for first curve click
    clicked = b1.checkClick(xMouse, HEIGHT - yMouse, 12);

    if (!clicked)
      // check for second curve click
      clicked = b2.checkClick(xMouse, HEIGHT - yMouse, 12);

    if (!clicked)
    {
      // check for slider click
      slider.checkClick(xMouse, HEIGHT - yMouse, 12);

      if (!slider.isDragging())
      {
        // first curve has less than 2 points
        if (b1.getPoints() < 2)
        {
          b1.addPoint(xMouse, HEIGHT - yMouse);
        }
        // first curve has more than 2 points, but less than all points
        else if (b1.getPoints() < 5)
        {
          // add point at click location
          b1.addPoint(xMouse, HEIGHT - yMouse);

          // calculate vector between second and first point
          Vector2D q(b1.controlPoints[1], b1.controlPoints[0]);

          // translate first point with vector & add
          b1.addPoint(b1.controlPoints[0].translated(q));

          // set color and disable point
          b1.controlPoints.back().color = Utils::LIGHT_GRAY;
          b1.controlPoints.back().disabled = true;

          // add last point (identical to first) & disable
          b1.addPoint(b1.controlPoints[0]);
          b1.controlPoints.back().disabled = true;
        }
        // second curve has no points
        else if (b2.getPoints() < 1)
        {
          b2.addPoint(xMouse, HEIGHT - yMouse);
        }
        // second curve has more than 1 points, but less than all points
        else if (b2.getPoints() < 6)
        {
          // save previously placed point
          auto r2 = b2.controlPoints.back();

          // remove previous point
          b2.controlPoints.pop_back();

          // save clicked point
          Point2D r3(xMouse, HEIGHT - yMouse);

          // add first point (identical to first curve's last) & disable
          b2.addPoint(b1.controlPoints[0]);
          b2.controlPoints.back().disabled = true;

          // calculate vector between fist curve's first and second point
          Vector2D q(b1.controlPoints[0], b1.controlPoints[1]);

          // translate first curve's first point w/ vector & multiply with
          // first curve's degree / second curve's degree (4/5 = 0.8)
          b2.addPoint(b1.controlPoints[0].translated(0.8 * q));

          // set color & disable point
          b2.controlPoints.back().color = Utils::LIGHT_GRAY;
          b2.controlPoints.back().disabled = true;

          // move in previously saved points
          b2.movePoint(r2);
          b2.movePoint(r3);

          // translate first curve's first point w/ -vector & multiply with
          // first curve's degree / second curve's degree (4/5 = 0.8)
          b2.addPoint(b2.controlPoints[0].translated(0.8 * -q));

          // set color & disable point
          b2.controlPoints.back().color = Utils::LIGHT_GRAY;
          b2.controlPoints.back().disabled = true;

          // add last point (identical to first curve's first point) & disable
          b2.addPoint(b1.controlPoints[0]);
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

// ----------------------------------------------------------------------------
// Main mouse movement handler
// ----------------------------------------------------------------------------
void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
  if (clicked)
  {
    if (b1.clicked)
    {
      // handle clicked point on first curve
      b1.handleClick(xMouse, HEIGHT - yMouse, clicked);

      // calculate vector between second and first point
      Vector2D q(b1.controlPoints[1], b1.controlPoints[0]);

      // move penultimate point with vector's opposite
      b1.controlPoints[3].setXY(b1.controlPoints[1].translated(2 * q));

      // stick last point to first
      b1.controlPoints.back().setXY(b1.controlPoints[0]);

      // second curve is not empty
      if (b2.getPoints())
      {
        // stick first point to first curve's first point
        b2.controlPoints[0].setXY(b1.controlPoints[0]);

        // translate first curve's second point w/ vector & multiply with
        // 1 - first curve's degree / second curve's degree (1 - 4/5 = 0.2)
        b2.controlPoints[1].setXY(b1.controlPoints[1].translated(0.2 * q));

        // translate first curve's second point w/ vector & multiply with
        // first curve's degree / second curve's degree (4/5 = 0.8)
        b2.controlPoints[4].setXY(b1.controlPoints[4].translated(0.8 * q));

        // stick last point to first curve's first point
        b2.controlPoints.back().setXY(b1.controlPoints[0]);
      }
    }

    if (b2.clicked)
      // handle clicked point on second curve
      b2.handleClick(xMouse, HEIGHT - yMouse, clicked);

    glutPostRedisplay();
  }

  // handle slider
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
