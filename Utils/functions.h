#pragma once
#include <cmath>

namespace Utils
{

const GLdouble PI = 3.14159265358979323846;

double degToRad(double deg)
{
  return deg * (Utils::PI / 180);
}

// ----------------------------------------------------------------------------
// Calculate binomial coefficient [recursive]
// ----------------------------------------------------------------------------
size_t binomialCoeff(size_t n, size_t k)
{
  if (k == 0 || k == n)
    return 1;

  return binomialCoeff(n - 1, k - 1) + binomialCoeff(n - 1, k);
}

// ----------------------------------------------------------------------------
// Returns value of the Bernstein polynomial.
// ----------------------------------------------------------------------------
double bernsteinPolynomial(size_t n, size_t i, double t)
{
  return binomialCoeff(n, i) * std::pow(t, i) * std::pow(1 - t, n - i);
}

} // end namespace Utils
