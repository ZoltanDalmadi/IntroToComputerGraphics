#include <GL/freeglut.h>
#include <sstream>
#include <string>
#include <memory>
#include "Rectangle.h"
#include "Matrix.h"
#include "Point3D.h"
#include "Color.h"
#include "Vector2D.h"
#include "Sphere.h"
#include "Torus.h"
#include "Button.h"

// ----------------------------------------------------------------------------
// Window size
// ----------------------------------------------------------------------------
const GLsizei WIDTH = 1280;
const GLsizei HEIGHT = 720;

// ----------------------------------------------------------------------------
// Colors
// ----------------------------------------------------------------------------
const Utils::Color bgColor(Utils::WHITE);
const Utils::Color normalColor(Utils::MAGENTA);
const Utils::Color pointColor(Utils::DARK_RED);
const Utils::Color edgeColor(Utils::BLACK);

// ----------------------------------------------------------------------------
// Miscellaneous variables
// ----------------------------------------------------------------------------
bool drag = false;
bool lightDrag = false;
double lastRotX = 0.0f;
double lastRotY = 0.0f;
double lastLightX = 0.0f;
double lastLightY = 0.0f;
GLint clickedX;
GLint clickedY;
GLint lightX;
GLint lightY;

// ----------------------------------------------------------------------------
// Window and viewport
// ----------------------------------------------------------------------------
Utils::Rectangle<GLdouble> window(-1.5, -1.5, 1.5, 1.5);
Utils::Rectangle<GLdouble> viewport(280, 0, 280 + HEIGHT, HEIGHT);

// ----------------------------------------------------------------------------
// Matrices
// ----------------------------------------------------------------------------
Utils::CentralProjection<GLdouble> cp(8.0f);
Utils::WindowToViewport<GLdouble> wtv(window, viewport);
auto projTrans = wtv * cp;
Utils::Rotate3DX<GLdouble> rx(0);
Utils::Rotate3DY<GLdouble> ry(0);
auto ryrx = ry * rx;
Utils::Point3DH<GLdouble> centerofProjection(0, 0, 8.0f, 1);
Utils::Point3DH<GLdouble> lightSource(2, 2, 8, 1);

// ----------------------------------------------------------------------------
// Info text
// ----------------------------------------------------------------------------
std::string tText;
std::stringstream ss;

// ----------------------------------------------------------------------------
// Buttons
// ----------------------------------------------------------------------------
Utils::Button normalsButton("Normals", 16, 208, 156, 32);
Utils::Button pointsButton("Points", 16, 160, 156, 32);
Utils::Button shadingButton("Shading", 16, 112, 156, 32);
Utils::Button edgesButton("Wireframe", 16, 64, 156, 32);
Utils::Button cullButton("Backface culling", 16, 16, 156, 32);

Utils::Button objectsButton("Object", 1100, 668, 160, 32);

// ----------------------------------------------------------------------------
// The sphere
// ----------------------------------------------------------------------------
std::shared_ptr<Utils::Mesh<GLdouble>> activeObject;
std::vector<std::shared_ptr<Utils::Mesh<GLdouble>>> objects;
auto objIterator = objects.begin();

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

  objects.emplace_back(new Utils::Sphere<GLdouble>());
  objects.back()->pointSize = 6.0f;
  objects.back()->normalColor = normalColor;
  objects.back()->pointColor = pointColor;
  objects.back()->edgeColor = edgeColor;

  objects.emplace_back(new Utils::Torus<GLdouble>());
  objects.back()->pointSize = 6.0f;
  objects.back()->normalColor = normalColor;
  objects.back()->pointColor = pointColor;
  objects.back()->edgeColor = edgeColor;

  normalsButton.setPaddingX(46);
  pointsButton.setPaddingX(52);
  shadingButton.setPaddingX(48);
  edgesButton.setPaddingX(36);

  activeObject = objects.front();
  objIterator = std::next(objects.begin());

  objectsButton.setLabel(activeObject->label);
}

// ----------------------------------------------------------------------------
// Shows current value of projection distance and number of segments
// ----------------------------------------------------------------------------
void drawInfoText(GLint x, GLint y, const Utils::Color& color)
{
  ss << "Projection distance: " << cp.getDistanceToOrigin() << std::endl;
  ss << "Segments: " << activeObject->getSegments() << std::endl;

  if (activeObject->backfaceCull)
    ss << "Backface culling: " << "on" << std::endl;
  else
    ss << "Backface culling: " << "off" << std::endl;

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

  activeObject->drawFaces(projTrans, ryrx, centerofProjection, lightSource);

  edgesButton.draw();
  normalsButton.draw();
  pointsButton.draw();
  cullButton.draw();
  shadingButton.draw();

  objectsButton.draw();

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

    if (edgesButton.hover(xMouse, HEIGHT - yMouse))
    {
      edgesButton.setColor(Utils::RED);
      activeObject->drawEdges == true ?
      activeObject->drawEdges = false : activeObject->drawEdges = true;
    }

    if (normalsButton.hover(xMouse, HEIGHT - yMouse))
    {
      normalsButton.setColor(Utils::RED);
      activeObject->drawNormals == true ?
      activeObject->drawNormals = false : activeObject->drawNormals = true;
    }

    if (pointsButton.hover(xMouse, HEIGHT - yMouse))
    {
      pointsButton.setColor(Utils::RED);
      activeObject->drawPoints == true ?
      activeObject->drawPoints = false : activeObject->drawPoints = true;
    }

    if (cullButton.hover(xMouse, HEIGHT - yMouse))
    {
      cullButton.setColor(Utils::RED);
      activeObject->backfaceCull == true ?
      activeObject->backfaceCull = false : activeObject->backfaceCull = true;
    }

    if (shadingButton.hover(xMouse, HEIGHT - yMouse))
    {
      shadingButton.setColor(Utils::RED);
      activeObject->filled == true ?
      activeObject->filled = false : activeObject->filled = true;
    }

    if (objectsButton.hover(xMouse, HEIGHT - yMouse))
    {
      objectsButton.setColor(Utils::RED);

      activeObject = *objIterator++;

      if (objIterator == objects.end())
        objIterator = objects.begin();

      objectsButton.setLabel(activeObject->label);

    }

    glutPostRedisplay();
  }

  if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
  {
    drag = false;

    // cache current rotation values
    lastRotX = rx.getAngle();
    lastRotY = ry.getAngle();

    if (edgesButton.hover(xMouse, HEIGHT - yMouse))
      edgesButton.setColor(Utils::BLACK);

    if (normalsButton.hover(xMouse, HEIGHT - yMouse))
      normalsButton.setColor(Utils::BLACK);

    if (pointsButton.hover(xMouse, HEIGHT - yMouse))
      pointsButton.setColor(Utils::BLACK);

    if (cullButton.hover(xMouse, HEIGHT - yMouse))
      cullButton.setColor(Utils::BLACK);

    if (shadingButton.hover(xMouse, HEIGHT - yMouse))
      shadingButton.setColor(Utils::BLACK);

    if (objectsButton.hover(xMouse, HEIGHT - yMouse))
      objectsButton.setColor(Utils::BLACK);

    glutPostRedisplay();
  }

  if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
  {
    lightDrag = true;

    // cache clicked point position
    lightX = xMouse;
    lightY = HEIGHT - yMouse;
  }

  if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
  {
    lightDrag = false;

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
    ryrx = ry * rx;
  }

  if (lightDrag)
  {
    Utils::Vector2D<GLint> v(lightX, lightY, xMouse, HEIGHT - yMouse);
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
    projTrans = wtv * cp;
  }

  if (key == GLUT_KEY_DOWN)
  {
    auto old = cp.getDistanceToOrigin();
    cp.setDistanceToOrigin(old + 0.1f);
    centerofProjection.setZ(old + 0.1f);
    projTrans = wtv * cp;
  }

  glutPostRedisplay();
}

// ----------------------------------------------------------------------------
// Segmentation control
// ----------------------------------------------------------------------------
void segmentationControl(unsigned char key, int x, int y)
{
  if (key == 'w')
    activeObject->increaseSegments();
  else if (key == 's' && activeObject->getSegments() > 2)
    activeObject->decreaseSegments();

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
  glutKeyboardFunc(segmentationControl);
  glutMainLoop();
  return 0;
}
