#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Window size.
const GLsizei WIDTH = 800;
const GLsizei HEIGHT = 800;

void init() {

}

void display() {
  
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Homework 01");

  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
