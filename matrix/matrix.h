#ifndef MATRIX_H
#define MATRIX_H

#include "device.h"

template <typename T, int M, int N, typename D = DevCpu>
class Matrix {
 public:
  using DeviceType = D;
  using ElemType = T;
  static int NumOfRow = M;
  static int NumOfCol = N;

 public:
  Matrix() {
    ptr_ = new T[M * N];
  }

  ~Matrix() {
    delete[] ptr_;
  }

 private:
  T* ptr_;
};

#endif  // MATRIX_H
