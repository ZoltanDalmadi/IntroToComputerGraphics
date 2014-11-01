#pragma once

#include <iostream>

namespace Utils {

template <size_t N, size_t M, typename T>
class Matrix {
protected:
  T data[N][M];

public:
  Matrix() {}

  Matrix(const Matrix<N, M, T>& rhs) {
    for(size_t col = 0; col < N; ++col)
      for(size_t row = 0; row < M; ++row)
        data[col][row] = rhs.data[col][row];
  }

  explicit Matrix(const T *values) {
    for(size_t col = 0; col < N; ++col)
      for(size_t row = 0; row < M; ++row)
        data[col][row] = values[row * N + col];
  }

  virtual ~Matrix() {}
  
  const T& operator()(size_t row, size_t column) const {
    if(row >= 0 && row < M && column >= 0 && column < N)
      return data[column][row];
  }

  T& operator()(size_t row, size_t column) {
    if(row >= 0 && row < M && column >= 0 && column < N)
      return data[column][row];
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

  template<size_t NN, size_t M1, size_t M2, typename TT>
  friend Matrix<M1, M2, TT> operator*(const Matrix<NN, M2, TT>& m1,
                                      const Matrix<M1, NN, TT>& m2) {

    Matrix<M1, M2, T> result;
    for(size_t row = 0; row < M2; ++row) {
      for(size_t col = 0; col < M1; ++col) {
        T sum = 0.0f;
        for(size_t j = 0; j < N; ++j)
          sum += m1.data[j][row] * m2.data[col][j];
        result.data[col][row] = sum;
      }
    }
    return result;
  }

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

} // end namespace Utils
