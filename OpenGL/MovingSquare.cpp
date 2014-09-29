#include <GL/glut.h>

GLint novX = 1;
GLint halfSize = 50;
GLint center[ 2 ] = { 200, 150 };

void init( ) {
    glClearColor ( 1.0, 1.0, 1.0, 0.0 );
    glMatrixMode ( GL_PROJECTION );
    gluOrtho2D ( 0.0, 400.0, 0.0, 300.0 );
    glShadeModel ( GL_FLAT );
}

void display( )
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 1.0);
   glRecti( center[ 0 ] - halfSize, center[ 1 ] - halfSize, center[ 0 ] + halfSize, center[ 1 ] + halfSize );
   glutSwapBuffers();
}

void update( int n )
{  center[ 0 ] += novX;
   if ( center[ 0 ] + halfSize > 400 || center[ 0 ] - halfSize < 0  )
        novX *= -1;
   glutPostRedisplay( );

   glutTimerFunc( 5, update, 0 );

}

void keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
        case 27:
            exit( 0 );
            break;
        case 'r':
            glClearColor ( 1.0, 0.0, 0.0, 0.0 );
            break;
        case 'g':
            glClearColor ( 0.0, 1.0, 0.0, 0.0 );
            break;
    }
}

int main(int argc, char** argv)
{
   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
   glutInitWindowSize( 400, 300 );
   glutInitWindowPosition ( 100, 100 );
   glutCreateWindow( argv[ 0 ] );
   init( );
   glutDisplayFunc( display );
   glutKeyboardFunc( keyboard );
   glutTimerFunc( 5, update, 0 );
   glutMainLoop( );
   return 0;
}
