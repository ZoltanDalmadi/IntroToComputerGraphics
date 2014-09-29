#include <GL/glut.h>
#include <iostream>
#include "Utils.h"

const int WIDTH = 640;
const int HEIGHT = 640;

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, GLdouble(WIDTH), 0.0, GLdouble(HEIGHT));
}

void drawGrid(int width, int height, int gap, GLfloat lineWidth,
              GLubyte* color) {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3ubv(color);
  glLineWidth(lineWidth);
  glBegin(GL_LINES);
  for(int i = gap; i < width; i += gap) {
    glVertex2i(i, 0);
    glVertex2i(i, height);
  }

  for(int i = gap; i < height; i += gap) {
    glVertex2i(0, i);
    glVertex2i(width, i);
  }

  glEnd();

  glFlush();
}

void drawThing(int width, int height, int x, int y, int gap, int lineWidth,
               GLubyte* color) {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3ubv(color);
  glLineWidth(lineWidth);
  glBegin(GL_LINES);

  // draw axis
  glVertex2i(x, y);
  glVertex2i(x, y + height);
  glVertex2i(x, y);
  glVertex2i(x + width, y);

  size_t notches = height / gap;

  for(size_t i = 0; i < notches; i++) {
    glVertex2i(x, y + gap*i);
    glVertex2i(x + width - gap*i, y);
  }

  glEnd();
  glFlush();
}

void lineSegment(void) {
  //GLubyte gridColor[3] = { 45, 45, 45 };
  //drawGrid(WIDTH, HEIGHT, 50, 1, gridColor);
  GLubyte thingColor[3] = { 0, 0, 0 };
  drawThing(600, 600, 20, 20, 20, 1, thingColor);
  Utils::Line *line = new Utils::Line(20, 30, 320, 240);
  std::cout << line->length() << std::endl;
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  //glutInitWindowPosition(50, 100);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Thing");

  init();
  glutDisplayFunc(lineSegment);
  glutMainLoop();
  return 0;
}