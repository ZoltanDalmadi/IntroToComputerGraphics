#pragma once
#include <GL/glut.h>

namespace Utils {

// Predefined colors.
enum namedColor
{
  BLACK,
  WHITE,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  MAGENTA,
  CYAN,
  MEDIUM_GRAY,
  LIGHT_GRAY,
  DARK_RED,
  DARK_GREEN,
  DARK_BLUE,
  DARK_CYAN,
  DARK_MAGENTA,
  DARK_YELLOW
};

class Color
{
private:
  // Color components.
  int r;
  int g;
  int b;
  int a;

public:
  // Initialize with RGBA values.
  Color(int r, int g, int b, int a = 0) : r(r), g(g), b(b), a(a) {}

  // Initialize with a predefined color.
  Color(namedColor color) {
    int colors[][4] = {
        { 0, 0, 0, 0 }, // BLACK
        { 255, 255, 255, 0 }, // WHITE
        { 255, 0, 0, 0 }, // RED
        { 0, 255, 0, 0 }, // GREEN
        { 0, 0, 255, 0 }, // BLUE
        { 255, 255, 0, 0 }, // YELLOW
        { 255, 0, 255, 0 }, // MAGENTA
        { 0, 255, 255, 0 }, // CYAN
        { 128, 128, 128, 0 }, // MEDIUM_GRAY
        { 160, 160, 160, 0 }, // LIGHT_GRAY
        { 128, 0, 0, 0 }, // DARK_RED
        { 0, 128, 0, 0 }, // DARK_GREEN
        { 0, 0, 128, 0 }, // DARK_BLUE
        { 0, 128, 128, 0 }, // DARK_CYAN
        { 128, 0, 128, 0 }, // DARK_MAGENTA
        { 128, 128, 0, 0 }  // DARK_YELLOW
    };

    r = colors[color][0];
    g = colors[color][1];
    b = colors[color][2];
    a = colors[color][3];
  }

  // Returns red component.
  inline int red() const {
    return r;
  }

  // Returns green component.
  inline int green() const {
    return g;
  }

  // Returns blue component.
  inline int blue() const {
    return b;
  }

  // Returns alpha component.
  inline int alpha() const {
    return a;
  }

  // Set active OpenGL color.
  inline void setGLColor() const {
    glColor3ub(r, g, b);
  }

  // Set OpenGL clear color.
  inline void setGLClearColor() const {
    glClearColor(static_cast<GLclampf>(r) / 255,
                 static_cast<GLclampf>(g) / 255,
                 static_cast<GLclampf>(b) / 255,
                 static_cast<GLclampf>(a) / 255);
  }

}; // end class Color

} // end namespace Utils
