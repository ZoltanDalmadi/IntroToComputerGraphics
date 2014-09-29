#include <GL/glut.h>

GLint x0 = 100, y0 = 200, x1 = 200, y1 = 100;

void init() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 0.0, 1.0);

  glBegin(GL_LINES);
  glVertex2i(x0, y0);
  glVertex2i(x1, y1);
  glEnd();

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
    exit(0);
    break;
    case 'w':
    y0++;
    break;
    case 's':
    y0--;
    break;
    case 'a':
    x0--;
    break;
    case 'd':
    x0++;
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(400, 300);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
