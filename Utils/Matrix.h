#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "Rectangle.h"

namespace Utils
{

template <typename T>
class Matrix
{
protected:
  size_t rows;
  size_t cols;
  std::vector<std::vector<T>> data;

public:
  inline size_t getRows() const
  {
    return this->rows;
  }

  inline size_t getCols() const
  {
    return this->cols;
  }

  Matrix(size_t N, size_t M) : rows(N), cols(M)
  {
    data.resize(M);

    for (size_t i = 0; i < data.size(); ++i)
    {
      data[i].resize(N);
    }
  }

  explicit Matrix(size_t N, size_t M, const T *values) : Matrix(N, M)
  {
    for (size_t col = 0; col < this->cols; ++col)
      for (size_t row = 0; row < this->rows; ++row)
        data[col][row] = values[row * this->cols + col];
  }

  virtual ~Matrix()
  {
  }

  inline const T& operator()(size_t row, size_t column) const
  {
    if (row >= 0 && row < this->rows && column >= 0 && column < this->cols)
      return data[column][row];
    else
      return data[0][0];
  }

  inline T& operator()(size_t row, size_t column)
  {
    if (row >= 0 && row < this->rows && column >= 0 && column < this->cols)
      return data[column][row];
    else
      return data[0][0];
  }

  void setToIdentity()
  {
    for (size_t col = 0; col < this->cols; ++col)
    {
      for (size_t row = 0; row < this->rows; ++row)
      {
        if (row == col)
          data[col][row] = 1.0f;
        else
          data[col][row] = 0.0f;
      }
    }
  }

  void print(std::ostream& os)
  {
    for (size_t row = 0; row < this->rows; ++row)
    {
      os << "|";

      for (size_t col = 0; col < this->cols; ++col)
      {
        os << data[col][row];

        if (col == this->cols - 1)
          os << "|" << std::endl;
        else
          os << " ";
      }
    }
  }

  Matrix<T>& operator*=(T factor)
  {
    for (size_t row = 0; row < this->rows; ++row)
      for (size_t col = 0; col < this->cols; ++col)
        data[col][row] *= factor;

    return *this;
  }

  Matrix<T> operator*(const Matrix<T>& rhs)
  {
    Matrix<T> result(this->rows, rhs.cols);

    for (size_t row = 0; row < this->rows; ++row)
    {
      for (size_t col = 0; col < rhs.cols; ++col)
      {
        T sum = 0.0f;

        for (size_t j = 0; j < this->cols; ++j)
          sum += this->data[j][row] * rhs.data[col][j];

        result.data[col][row] = sum;
      }
    }

    return result;
  }

  // 4x4 only
  Matrix<T> inverse()
  {
    Matrix<T> inv(this->rows, this->cols);
    inv.data[0][0] = data[1][1] * data[2][2] * data[3][3] -
                     data[1][1] * data[3][2] * data[2][3] -
                     data[1][2] * data[2][1] * data[3][3] +
                     data[1][2] * data[3][1] * data[2][3] +
                     data[1][3] * data[2][1] * data[3][2] -
                     data[1][3] * data[3][1] * data[2][2];

    inv.data[0][1] = -data[0][1] * data[2][2] * data[3][3] +
                     data[0][1] * data[3][2] * data[2][3] +
                     data[0][2] * data[2][1] * data[3][3] -
                     data[0][2] * data[3][1] * data[2][3] -
                     data[0][3] * data[2][1] * data[3][2] +
                     data[0][3] * data[3][1] * data[2][2];

    inv.data[0][2] = data[0][1] * data[1][2] * data[3][3] -
                     data[0][1] * data[3][2] * data[1][3] -
                     data[0][2] * data[1][1] * data[3][3] +
                     data[0][2] * data[3][1] * data[1][3] +
                     data[0][3] * data[1][1] * data[3][2] -
                     data[0][3] * data[3][1] * data[1][2];

    inv.data[0][3] = -data[0][1] * data[1][2] * data[2][3] +
                     data[0][1] * data[2][2] * data[1][3] +
                     data[0][2] * data[1][1] * data[2][3] -
                     data[0][2] * data[2][1] * data[1][3] -
                     data[0][3] * data[1][1] * data[2][2] +
                     data[0][3] * data[2][1] * data[1][2];

    inv.data[1][0] = -data[1][0] * data[2][2] * data[3][3] +
                     data[1][0] * data[3][2] * data[2][3] +
                     data[1][2] * data[2][0] * data[3][3] -
                     data[1][2] * data[3][0] * data[2][3] -
                     data[1][3] * data[2][0] * data[3][2] +
                     data[1][3] * data[3][0] * data[2][2];

    inv.data[1][1] = data[0][0] * data[2][2] * data[3][3] -
                     data[0][0] * data[3][2] * data[2][3] -
                     data[0][2] * data[2][0] * data[3][3] +
                     data[0][2] * data[3][0] * data[2][3] +
                     data[0][3] * data[2][0] * data[3][2] -
                     data[0][3] * data[3][0] * data[2][2];

    inv.data[1][2] = -data[0][0] * data[1][2] * data[3][3] +
                     data[0][0] * data[3][2] * data[1][3] +
                     data[0][2] * data[1][0] * data[3][3] -
                     data[0][2] * data[3][0] * data[1][3] -
                     data[0][3] * data[1][0] * data[3][2] +
                     data[0][3] * data[3][0] * data[1][2];

    inv.data[1][3] = data[0][0] * data[1][2] * data[2][3] -
                     data[0][0] * data[2][2] * data[1][3] -
                     data[0][2] * data[1][0] * data[2][3] +
                     data[0][2] * data[2][0] * data[1][3] +
                     data[0][3] * data[1][0] * data[2][2] -
                     data[0][3] * data[2][0] * data[1][2];

    inv.data[2][0] = data[1][0] * data[2][1] * data[3][3] -
                     data[1][0] * data[3][1] * data[2][3] -
                     data[1][1] * data[2][0] * data[3][3] +
                     data[1][1] * data[3][0] * data[2][3] +
                     data[1][3] * data[2][0] * data[3][1] -
                     data[1][3] * data[3][0] * data[2][1];

    inv.data[2][1] = -data[0][0] * data[2][1] * data[3][3] +
                     data[0][0] * data[3][1] * data[2][3] +
                     data[0][1] * data[2][0] * data[3][3] -
                     data[0][1] * data[3][0] * data[2][3] -
                     data[0][3] * data[2][0] * data[3][1] +
                     data[0][3] * data[3][0] * data[2][1];

    inv.data[2][2] = data[0][0] * data[1][1] * data[3][3] -
                     data[0][0] * data[3][1] * data[1][3] -
                     data[0][1] * data[1][0] * data[3][3] +
                     data[0][1] * data[3][0] * data[1][3] +
                     data[0][3] * data[1][0] * data[3][1] -
                     data[0][3] * data[3][0] * data[1][1];

    inv.data[2][3] = -data[0][0] * data[1][1] * data[2][3] +
                     data[0][0] * data[2][1] * data[1][3] +
                     data[0][1] * data[1][0] * data[2][3] -
                     data[0][1] * data[2][0] * data[1][3] -
                     data[0][3] * data[1][0] * data[2][1] +
                     data[0][3] * data[2][0] * data[1][1];

    inv.data[3][0] = -data[1][0] * data[2][1] * data[3][2] +
                     data[1][0] * data[3][1] * data[2][2] +
                     data[1][1] * data[2][0] * data[3][2] -
                     data[1][1] * data[3][0] * data[2][2] -
                     data[1][2] * data[2][0] * data[3][1] +
                     data[1][2] * data[3][0] * data[2][1];

    inv.data[3][1] = data[0][0] * data[2][1] * data[3][2] -
                     data[0][0] * data[3][1] * data[2][2] -
                     data[0][1] * data[2][0] * data[3][2] +
                     data[0][1] * data[3][0] * data[2][2] +
                     data[0][2] * data[2][0] * data[3][1] -
                     data[0][2] * data[3][0] * data[2][1];

    inv.data[3][2] = -data[0][0] * data[1][1] * data[3][2] +
                     data[0][0] * data[3][1] * data[1][2] +
                     data[0][1] * data[1][0] * data[3][2] -
                     data[0][1] * data[3][0] * data[1][2] -
                     data[0][2] * data[1][0] * data[3][1] +
                     data[0][2] * data[3][0] * data[1][1];

    inv.data[3][3] = data[0][0] * data[1][1] * data[2][2] -
                     data[0][0] * data[2][1] * data[1][2] -
                     data[0][1] * data[1][0] * data[2][2] +
                     data[0][1] * data[2][0] * data[1][2] +
                     data[0][2] * data[1][0] * data[2][1] -
                     data[0][2] * data[2][0] * data[1][1];

    T det = data[0][0] * inv.data[0][0] +
            data[1][0] * inv.data[0][1] +
            data[2][0] * inv.data[0][2] +
            data[3][0] * inv.data[0][3];

    det = 1.0f / det;

    inv *= det;
    return inv;
  }

}; // end class Matrix

template <typename T>
class Translate2D : public Matrix<T>
{
protected:
  T deltaX;
  T deltaY;

  void updateTransform()
  {
    this->data[2][0] = deltaX;
    this->data[2][1] = deltaY;
  }

public:
  Translate2D(T delta1, T delta2)
    : Matrix<T>(3, 3), deltaX(delta1), deltaY(delta2)
  {
    this->data[0][0] = 1.0f;
    this->data[1][1] = 1.0f;
    this->data[2][2] = 1.0f;
    updateTransform();
  }

  virtual ~Translate2D()
  {
  }

  inline void setDeltaX(double delta)
  {
    this->deltaX = delta;
    updateTransform();
  }

  inline void setDeltaY(double delta)
  {
    this->deltaY = delta;
    updateTransform();
  }

  inline T getDeltaX() const
  {
    return deltaX;
  }

  inline T getDeltaY() const
  {
    return deltaY;
  }

}; // end class Translate2D

template <typename T>
class Translate3D : public Matrix<T>
{
protected:
  T deltaX;
  T deltaY;
  T deltaZ;

  void updateTransform()
  {
    this->data[2][0] = deltaX;
    this->data[2][1] = deltaY;
    this->data[3][2] = deltaZ;
  }

public:
  Translate3D(T delta1, T delta2, T delta3)
    : Matrix<T>(4, 4), deltaX(delta1), deltaY(delta2), deltaZ(delta3)
  {
    this->data[0][0] = 1.0f;
    this->data[1][1] = 1.0f;
    this->data[2][2] = 1.0f;
    this->data[3][3] = 1.0f;
    updateTransform();
  }

  virtual ~Translate3D()
  {
  }

  inline void setDeltaX(double delta)
  {
    this->deltaX = delta;
    updateTransform();
  }

  inline void setDeltaY(double delta)
  {
    this->deltaY = delta;
    updateTransform();
  }

  inline void setDeltaZ(double delta)
  {
    this->deltaZ = delta;
    updateTransform();
  }

  inline T getDeltaX() const
  {
    return deltaX;
  }

  inline T getDeltaY() const
  {
    return deltaY;
  }

  inline T getDeltaZ() const
  {
    return deltaY;
  }

}; // end class Translate3D

template <typename T>
class Scale2D : public Matrix<T>
{
protected:
  double Xfactor; // he-he
  double Yfactor;

  void updateTransform()
  {
    this->data[0][0] = Xfactor;
    this->data[1][1] = Yfactor;
  }

public:
  /// Uniform scale
  Scale2D(double lambda)
    : Matrix<T>(3, 3), Xfactor(lambda), Yfactor(lambda)
  {
    this->data[2][2] = 1.0f;
    updateTransform();
  }

  /// Non-Uniform scale
  Scale2D(double lambda1, double lambda2)
    : Matrix<T>(3, 3), Xfactor(lambda1), Yfactor(lambda2)
  {
    this->data[2][2] = 1.0f;
    updateTransform();
  }

  virtual ~Scale2D()
  {
  }

  inline void setXFactor(double lambda)
  {
    this->Xfactor = lambda;
    updateTransform();
  }

  inline void setYFactor(double lambda)
  {
    this->Yfactor = lambda;
    updateTransform();
  }

  inline void setFactor(double lambda)
  {
    this->Xfactor = lambda;
    this->Yfactor = lambda;
    updateTransform();
  }

  inline double getXFactor() const
  {
    return Xfactor;
  }

  inline double getYFactor() const
  {
    return Yfactor;
  }

}; // end class Scale2D

template <typename T>
class Scale3D : public Matrix<T>
{
protected:
  double Xfactor;
  double Yfactor;
  double Zfactor;

  void updateTransform()
  {
    this->data[0][0] = Xfactor;
    this->data[1][1] = Yfactor;
    this->data[2][2] = Zfactor;
  }

public:
  /// Uniform scale
  Scale3D(double lambda)
    : Matrix<T>(4, 4), Xfactor(lambda), Yfactor(lambda), Zfactor(lambda)
  {
    this->data[3][3] = 1.0f;
    updateTransform();
  }

  /// Non-Uniform scale
  Scale3D(double lambda1, double lambda2, double lambda3)
    : Matrix<T>(4, 4), Xfactor(lambda1), Yfactor(lambda2), Zfactor(lambda3)
  {
    this->data[3][3] = 1.0f;
    updateTransform();
  }

  virtual ~Scale3D()
  {
  }

  inline void setXFactor(double lambda)
  {
    this->Xfactor = lambda;
    updateTransform();
  }

  inline void setYFactor(double lambda)
  {
    this->Yfactor = lambda;
    updateTransform();
  }

  inline void setZFactor(double lambda)
  {
    this->Zfactor = lambda;
    updateTransform();
  }

  inline void setFactor(double lambda)
  {
    this->Xfactor = lambda;
    this->Yfactor = lambda;
    this->Zfactor = lambda;
    updateTransform();
  }

  inline double getXFactor() const
  {
    return Xfactor;
  }

  inline double getYFactor() const
  {
    return Yfactor;
  }

  inline double getZFactor() const
  {
    return Zfactor;
  }

}; // end class Scale3D

template <typename T>
class Rotate2D : public Matrix<T>
{
protected:
  double angle;

  void updateTransform()
  {
    this->data[0][0] = cos(angle);
    this->data[0][1] = sin(angle);
    this->data[1][0] = -sin(angle);
    this->data[1][1] = cos(angle);
  }

public:
  Rotate2D(double alpha) : Matrix<T>(3, 3), angle(alpha)
  {
    this->data[2][2] = 1.0f;
    updateTransform();
  }

  virtual ~Rotate2D()
  {
  }

  inline void setAngle(double alpha)
  {
    this->angle = alpha;
    updateTransform();
  }

  inline double getAngle() const
  {
    return angle;
  }

}; // end class Rotate2D

template <typename T>
class Rotate3D : public Matrix<T>
{
protected:
  double angle;

  virtual void updateTransform()
  {
  }

public:
  Rotate3D(double alpha) : Matrix<T>(4, 4), angle(alpha)
  {
  }

  virtual ~Rotate3D()
  {
  }

  inline void setAngle(double alpha)
  {
    this->angle = alpha;
    this->updateTransform();
  }

  inline double getAngle() const
  {
    return angle;
  }

}; // end class Rotate3D

template <typename T>
class Rotate3DX : public Rotate3D<T>
{
protected:
  virtual void updateTransform()
  {
    this->data[1][1] = cos(angle);
    this->data[2][1] = -sin(angle);
    this->data[1][2] = sin(angle);
    this->data[2][2] = cos(angle);
  }

public:
  Rotate3DX(double alpha) : Rotate3D<T>(alpha)
  {
    this->data[0][0] = 1.0f;
    this->data[3][3] = 1.0f;
    this->updateTransform();
  }

  virtual ~Rotate3DX()
  {
  }

}; // end class Rotate3DX

template <typename T>
class Rotate3DY : public Rotate3D<T>
{
protected:
  void updateTransform()
  {
    this->data[0][0] = cos(angle);
    this->data[2][0] = sin(angle);
    this->data[0][2] = -sin(angle);
    this->data[2][2] = cos(angle);
  }

public:
  Rotate3DY(double alpha) : Rotate3D<T>(alpha)
  {
    this->data[1][1] = 1.0f;
    this->data[3][3] = 1.0f;
    this->updateTransform();
  }

  virtual ~Rotate3DY()
  {
  }

}; // end class Rotate3DY

template <typename T>
class Rotate3DZ : public Rotate3D<T>
{
protected:
  void updateTransform()
  {
    this->data[0][0] = cos(angle);
    this->data[1][0] = -sin(angle);
    this->data[0][1] = sin(angle);
    this->data[1][1] = cos(angle);
  }

public:
  Rotate3DZ(double alpha) : Rotate3D<T>(alpha)
  {
    this->data[2][2] = 1.0f;
    this->data[3][3] = 1.0f;
    this->updateTransform();
  }

  virtual ~Rotate3DZ()
  {
  }

}; // end class Rotate3DZ

template <typename T>
class PerpendicularProjection : public Matrix<T>
{
public:
  PerpendicularProjection()
    : Matrix<T>(4, 4)
  {
    this->data[0][0] = 1.0f;
    this->data[1][1] = 1.0f;
    this->data[3][3] = 1.0f;
  }

  virtual ~PerpendicularProjection()
  {
  }

}; // end class PerpendicularProjection

template <typename T>
class CentralProjection : public Matrix<T>
{
protected:
  T distanceToOrigin;

  void updateTransform()
  {
    this->data[2][3] = -1.0f / this->distanceToOrigin;
  }

public:
  CentralProjection(double z)
    : Matrix<T>(4, 4), distanceToOrigin(z)
  {
    this->data[0][0] = 1.0f;
    this->data[1][1] = 1.0f;
    this->data[3][3] = 1.0f;
    this->updateTransform();
  }

  virtual ~CentralProjection()
  {
  }

  inline void setDistanceToOrigin(double z)
  {
    this->distanceToOrigin = z;
    this->updateTransform();
  }

  inline double getDistanceToOrigin() const
  {
    return this->distanceToOrigin;
  }

}; // end class CentralProjection

template <typename T> class Rectangle;

template <typename T>
class WindowToViewport : public Matrix<T>
{
protected:
  Rectangle<T> window;
  Rectangle<T> viewport;

  void updateTransform()
  {
    data[0][0] = viewport.width() / window.width();
    data[1][1] = viewport.height() / window.height();
    data[3][0] = viewport.left() - window.left() * data[0][0];
    data[3][1] = viewport.bottom() - window.bottom() * data[1][1];
  }

public:
  WindowToViewport(const Rectangle<T>& window, const Rectangle<T>& viewport)
    : Matrix<T>(4, 4), window(window), viewport(viewport)
  {
    this->data[2][2] = 1.0f;
    this->data[3][3] = 1.0f;
    this->updateTransform();
  }

  WindowToViewport(T wblx, T wbly, T wtrx, T wtry,
                   T vblx, T vbly, T vtrx, T vtry)
    : Matrix<T>(4, 4), window(wblx, wbly, wtrx, wtry),
      viewport(vblx, vbly, vtrx, vtry)
  {
    this->data[2][2] = 1.0f;
    this->data[3][3] = 1.0f;
    this->updateTransform();
  }

  virtual ~WindowToViewport()
  {
  }

  inline void setWindow(const Rectangle<T>& rect)
  {
    this->window = std::move(rect);
    this->updateTransform();
  }

  inline void setViewport(const Rectangle<T>& rect)
  {
    this->viewport = std::move(rect);
    this->updateTransform();
  }

  inline Rectangle<T> getWindow() const
  {
    return this->window;
  }

  inline Rectangle<T> getViewport() const
  {
    return this->viewport;
  }

}; // end class WindowToViewport

} // end namespace Utils
