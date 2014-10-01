#include <GL/glut.h>
#include <iostream>
#include <cmath>

const int WIDTH = 640;
const int HEIGHT = 640;
const int CENTER_X = WIDTH / 2;
const int CENTER_Y = HEIGHT / 2;
const int RADIUS = 200;

const double M_PI = 3.14159265358979323846;

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glPointSize(10);
  glLineWidth(2);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void circle() {
  //GLubyte gridColor[3] = { 45, 45, 45 };
  //drawGrid(WIDTH, HEIGHT, 50, 1, gridColor);
  //GLubyte thingColor[3] = { 0, 0, 0 };
  //drawThing(600, 600, 20, 20, 40, 1, thingColor);
  size_t points = 128;
  double gap = 2 * M_PI / points;
  double gap2 = 2 * M_PI / 12;

  double c = cos(gap); //precalculate the sine and cosine
  double s = sin(gap);
  double t;

  double x = RADIUS; //we start at angle = 0 
  double y = 0;

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3ub(0, 0, 0);
  glBegin(GL_LINE_LOOP);
  for(int i = 0; i < points; i++) {
    glVertex2d(x + CENTER_X, y + CENTER_Y); //output vertex 

    //apply the rotation matrix
    t = x;
    x = c * x - s * y;
    y = s * t + c * y;
  }
  glEnd();
  //glBegin(GL_LINE_STRIP);
  //for(size_t i = 0; i <= points; ++i)
  //  glVertex2d(CENTER_X + RADIUS * cos(i*gap), CENTER_Y + RADIUS/2 * sin(i*gap));
  //glEnd();

  //glColor3ub(255, 0, 0);
  //glBegin(GL_POINTS);
  //for(size_t i = 0; i <= 12; ++i)
  //  glVertex2d(CENTER_X + RADIUS * cos(i*gap2), CENTER_Y + RADIUS/2 * sin(i*gap2));
  //glEnd();

  glFlush();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  //glutInitWindowPosition(50, 100);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Thing");

  init();
  glutDisplayFunc(circle);
  glutMainLoop();
  return 0;
}