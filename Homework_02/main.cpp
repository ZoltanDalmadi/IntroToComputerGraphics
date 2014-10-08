#include <GL/glut.h>

// Window size.
const GLsizei WIDTH = 800;
const GLsizei HEIGHT = 800;

void init() {

}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Homework 02");

  init();
  glutMainLoop();
  return 0;
}
