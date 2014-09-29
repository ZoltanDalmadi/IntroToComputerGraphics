#include <GL/glut.h>

GLfloat points[ 4 ][ 2 ] = { { 100.0, 100.0 } , { 200.0, 150.0 }, { 300.0, 175.0 }, { 350.0, 50.0 } };

void init( ) {
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 400.0, 0.0, 300.0);
}

void display( ) {
    int i;
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (0.0, 0.0, 0.0);

    glBegin (GL_LINE_LOOP);
	for ( i = 0; i < 4; i++ )
	    glVertex2fv ( points[ i ] );
    glEnd( );

    glFlush( );
}

int main (int argc, char** argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition (500, 100);
    glutInitWindowSize (400, 300);
    glutCreateWindow ("Polygon");
    init ( );
    glutDisplayFunc (display);
    glutMainLoop ( );
    return 0;
}
