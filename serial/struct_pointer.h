#ifndef STRUCT_POINTER_H
#define STRUCT_POINTER_H

#include <iostream>
#include <cstdint>
#include <cstring>
#include <type_traits>

template <typename T>
class alignas(8) StructPointer {
 public:
  template <typename ...A>
  StructPointer(A... args) : own_(true), ptr_(new T(args...)) {  };

  ~StructPointer() { if (own_) { delete ptr_; } }

  uint32_t DataSize() const {
    return sizeof(StructPointer) + ptr_->DataSize();
  }

  uint32_t Serialize(void* buf, uint32_t offset) const {
    StructPointer* p = (StructPointer*)buf;
    p->own_ = false;
    p->ptr_ = reinterpret_cast<T*>(offset);
    return ptr_->Serialize((char*)buf + offset);
  }

  T* Ptr() {
    if (own_) {
      return ptr_;
    } else {
      return (T*)((char*)this + (uintptr_t)ptr_);
    }
  }

  const T* Ptr() const {
    if (own_) {
      return ptr_;
    } else {
      return (T*)((char*)this + (uintptr_t)ptr_);
    }
  }

  T* operator->() { return Ptr(); }

  const T* operator->() const { return Ptr(); }

  T& Ref() { return *Ptr(); }

  const T& Ref() const { return *Ptr(); }

 private:
  bool own_;
  T* ptr_;
};

template <typename T>
class Vector {
 public:
  explicit Vector(uint32_t num) : own_(true), num_(num), ptr_(new T[num_]) {  }

  ~Vector() { if (own_) { delete[] ptr_; } }

  uint32_t DataSize() const {
    return sizeof(Vector) + num_ * sizeof(T);
  }

  uint32_t Serialize(void* buf) const {
    Vector* p = (Vector*)buf;
    std::memcpy((void*)p, this, sizeof(Vector));
    p->own_ = false;
    p->ptr_ = reinterpret_cast<T*>(sizeof(Vector));
    std::memcpy((char*)buf + (uintptr_t)p->ptr_, ptr_, num_ * sizeof(T));

    return DataSize();
  }

  T& operator[](uint32_t idx) {
    if (own_) {
      return ptr_[idx];
    } else {
      return ((T*)((char*)this + (uintptr_t)ptr_))[idx];
    }
  }

  uint32_t Num() const { return num_; }

 private:
  bool own_;
  uint32_t num_;
  T* ptr_;
};

template <typename T>
class Vector<StructPointer<T>> {
 public:
  explicit Vector(uint32_t num)
    : own_(true), num_(num), ptr_(new StructPointer<T>()) {  }

  ~Vector() { if (own_) { delete[] ptr_; } }

  uint32_t DataSize() const {
    uint32_t data_size = sizeof(Vector);
    for (uint32_t i = 0; i < num_; ++i) {
      data_size += ptr_->DataSize();
    }

    return data_size;
  }

  uint32_t Serialize(void* buf) const {
    Vector* p = (Vector*)buf;
    p->own_ = false;
    std::memcpy((void*)p, this, sizeof(Vector));
    p->ptr_ = reinterpret_cast<T*>(sizeof(Vector));

    uint32_t data_size = DataSize();
    for (size_t i = 0; i < num_; ++i) {
      
    }

    return data_size;
  }

  T& operator[](uint32_t idx) {
    if (own_) {
      return ptr_[idx].Ref();
    } else {
      return ((StructPointer<T>*)((char*)this + (uintptr_t)ptr_))[idx].Ref();
    }
  }

  uint32_t Num() const { return num_; }

 private:
  bool own_;
  uint32_t num_;
  StructPointer<T>* ptr_;
};

#endif  // STRUCT_POINTER_H
