#include <GL/glut.h>
#include <math.h>

typedef struct point2d {
  GLfloat x;
  GLfloat y;
} POINT2D;

GLsizei winWidth = 800, winHeight = 600;

POINT2D points[4] = { 100, 300, 200, 100, 300, 200, 400, 100 };

GLint dragged = -1;

GLint Round(GLfloat n) { return (GLint)(n + 0.5); }

POINT2D initPoint2D(GLfloat x, GLfloat y) {
  POINT2D P;
  P.x = x;
  P.y = y;
  return P;
}

void init() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, winWidth, 0.0, winHeight);
  glShadeModel(GL_FLAT);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_STIPPLE);
  glPointSize(10.0);
  glLineWidth(5.0);
  glLineStipple(1, 0xFF00);
}

void display() {
  GLint i;
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 1.0, 0.0);

  glBegin(GL_LINES);
  for(i = 0; i < 4; i++)
    glVertex2f(points[i].x, points[i].y);
  glEnd();

  glColor3f(1.0, 0.0, 0.0);

  glBegin(GL_POINTS);
  for(i = 0; i < 4; i++)
    glVertex2f(points[i].x, points[i].y);
  glEnd();

  glutSwapBuffers();
}

/*
 *  Ket pont tavolsaganak negyzetet adja vissza.
 */
GLfloat dist2(POINT2D P1, POINT2D P2) {
  GLfloat t1 = P1.x - P2.x;
  GLfloat t2 = P1.y - P2.y;
  return t1 * t1 + t2 * t2;
}

GLint getActivePoint1(POINT2D p[], GLint size, GLint sens, GLint x, GLint y) {
  GLint i, s = sens * sens;
  POINT2D P = initPoint2D(x, y);

  for(i = 0; i < size; i++)
    if(dist2(p[i], P) < s)
      return i;
  return -1;
}

GLint getActivePoint2(POINT2D *p, GLint size, GLint sens, GLint x, GLint y) {
  GLint i;
  for(i = 0; i < size; i++)
    if(fabs((*(p + i)).x - x) < sens && fabs((*(p + i)).y - y) < sens)
      return i;
  return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
  GLint i;
  if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    if((i = getActivePoint1(points, 4, 8, xMouse, winHeight - yMouse)) != -1)
      dragged = i;
  if(button == GLUT_LEFT_BUTTON && action == GLUT_UP)
    dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
  GLint i;
  if(dragged >= 0) {
    points[dragged].x = xMouse;
    points[dragged].y = winHeight - yMouse;
    glutPostRedisplay();
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(winWidth, winHeight);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Drag & Drop");
  init();
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}
