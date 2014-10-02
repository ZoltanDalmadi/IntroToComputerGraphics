#pragma once
#include "Ellipse.h"
namespace Utils {

template <typename T>
class Circle : public Ellipse<T>
{
public:

  Circle() {}
protected:
  ry = rx;

}; // end class Circle

} // endl namespace Utils
