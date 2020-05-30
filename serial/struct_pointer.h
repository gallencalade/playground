#ifndef STRUCT_POINTER_H
#define STRUCT_POINTER_H

template <typename T>
class StructPointer {
 public:
  template <typename ...A>
  StructPointer(A... args) : own_(true), ptr_(new T(args...)) {  };

 private:
  bool own_;
  T* ptr_;
};

#endif  // STRUCT_POINTER_H
