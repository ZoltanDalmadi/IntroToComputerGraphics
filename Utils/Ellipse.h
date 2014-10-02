#pragma once
namespace Utils {

template <typename T>
class Ellipse
{
public:

  Ellipse() {}

  ~Ellipse() {}
protected:
  T centre;
  T rx;
  T ry;
}; // end class Ellipse

} // end namespace Utils
