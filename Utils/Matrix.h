#pragma once

#include <iostream>

namespace Utils {

template <size_t N, size_t M, typename T>
class Matrix {
protected:
  T data[N][M];

public:
  Matrix() {}

  explicit Matrix(const T *values)
  {
    for(size_t col = 0; col < N; ++col)
      for(size_t row = 0; row < M; ++row)
        data[col][row] = values[row * N + col];
  }

  const T& operator()(size_t row, size_t column) const {
    if(row >= 0 && row < M && column >= 0 && column < N)
      return data[column][row];
    else
      return data[0][0];
  }

  T& operator()(size_t row, size_t column) {
    if(row >= 0 && row < M && column >= 0 && column < N)
      return data[column][row];
    else
      return data[0][0];
  }

  void setToIdentity() {
    for(size_t col = 0; col < N; ++col) {
      for(size_t row = 0; row < M; ++row) {
        if(row == col)
          data[col][row] = 1.0f;
        else
          data[col][row] = 0.0f;
      }
    }
  }

  void print(std::ostream& os) {
    for(size_t row = 0; row < M; ++row) {
      os << "|";

      for(size_t col = 0; col < N; ++col) {
        os << data[col][row];

        if(col == N - 1)
          os << "|" << std::endl;
        else
          os << " ";
      }
    }
  }

  Matrix<N, M, T>& operator*=(T factor) {
    for(size_t row = 0; row < M; ++row)
      for(size_t col = 0; col < N; ++col)
        data[col][row] *= factor;
    return *this;
  }

  template<int NN, int M1, int M2, typename TT>
  friend Matrix<M1, M2, TT> operator*(const Matrix<NN, M2, TT>& m1,
                                      const Matrix<M1, NN, TT>& m2);

  // 4x4 only
  Matrix<N, M, T> inverse() {
    Matrix<N, M, T> inv;
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

template <int N, int M1, int M2, typename T>
Matrix<M1, M2, T> operator*(const Matrix<N, M2, T>& m1,
                            const Matrix<M1, N, T>& m2)
{
  Matrix<M1, M2, T> result;
  for(int row = 0; row < M2; ++row)
  {
    for(int col = 0; col < M1; ++col)
    {
      T sum(0.0f);
      for(int j = 0; j < N; ++j)
        sum += m1.data[j][row] * m2.data[col][j];
      result.data[col][row] = sum;
    }
  }
  return result;
}

} // end namespace Utils
