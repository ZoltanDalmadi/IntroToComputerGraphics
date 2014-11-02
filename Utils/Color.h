#pragma once

#include <GL/glut.h>
#include <string>

namespace Utils {

// Predefined colors.
enum namedColor {
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
  VERY_LIGHT_GRAY,
  DARK_RED,
  DARK_GREEN,
  DARK_BLUE,
  DARK_CYAN,
  DARK_MAGENTA,
  DARK_YELLOW
};

class Color {
 private:
  // Color components.
  int r;
  int g;
  int b;
  int a;

 public:
  // Initialize with RGBA values.
  inline Color(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a) {}

  // Initialize with hex string.
  Color(std::string hexString) {
    size_t startpos = 0;

    if (hexString.front() == '#') {
      startpos = 1;
    }

    if (hexString.length() == 6 || hexString.length() == 7) {
      a = 0;
    }

    if (hexString.length() == 8 || hexString.length() == 9) {
      a = std::stoul(hexString.substr(startpos + 6, 2), 0, 16);
    }

    r = std::stoul(hexString.substr(startpos, 2), 0, 16);
    g = std::stoul(hexString.substr(startpos + 2, 2), 0, 16);
    b = std::stoul(hexString.substr(startpos + 4, 2), 0, 16);
  }

  // Initialize with a predefined color.
  Color(namedColor color) {
    int colors[][4] = {
      { 0, 0, 0, 255 }, // BLACK
      { 255, 255, 255, 255 }, // WHITE
      { 255, 0, 0, 255 }, // RED
      { 0, 255, 0, 255 }, // GREEN
      { 0, 0, 255, 255 }, // BLUE
      { 255, 255, 0, 255 }, // YELLOW
      { 255, 0, 255, 255 }, // MAGENTA
      { 0, 255, 255, 255 }, // CYAN
      { 128, 128, 128, 255 }, // MEDIUM_GRAY
      { 160, 160, 160, 255 }, // LIGHT_GRAY
      { 192, 192, 192, 255 }, // VERY_LIGHT_GRAY
      { 128, 0, 0, 255 }, // DARK_RED
      { 0, 128, 0, 255 }, // DARK_GREEN
      { 0, 0, 128, 255 }, // DARK_BLUE
      { 0, 128, 128, 255 }, // DARK_CYAN
      { 128, 0, 128, 255 }, // DARK_MAGENTA
      { 128, 128, 0, 255 }  // DARK_YELLOW
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
    glColor4ub(r, g, b, a);
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
