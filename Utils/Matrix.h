#pragma once

#include <iostream>
#include <vector>

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
    for (size_t col = 0; col < cols; ++col)
      for (size_t row = 0; row < rows; ++row)
        data[col][row] = values[row * cols + col];
  }

  const T& operator()(size_t row, size_t column) const
  {
    if (row >= 0 && row < rows && column >= 0 && column < cols)
      return data[column][row];
    else
      return data[0][0];
  }

  T& operator()(size_t row, size_t column)
  {
    if (row >= 0 && row < rows && column >= 0 && column < cols)
      return data[column][row];
    else
      return data[0][0];
  }

  void setToIdentity()
  {
    for (size_t col = 0; col < cols; ++col)
    {
      for (size_t row = 0; row < rows; ++row)
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
    for (size_t row = 0; row < rows; ++row)
    {
      os << "|";

      for (size_t col = 0; col < cols; ++col)
      {
        os << data[col][row];

        if (col == cols - 1)
          os << "|" << std::endl;
        else
          os << " ";
      }
    }
  }

  Matrix<T>& operator*=(T factor)
  {
    for (size_t row = 0; row < rows; ++row)
      for (size_t col = 0; col < cols; ++col)
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

} // end namespace Utils
