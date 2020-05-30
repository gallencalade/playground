#ifndef STRUCT_POINTER_H
#define STRUCT_POINTER_H

#include <iostream>
#include <cstdint>
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
  explicit Vector(uint32_t num) : num_(num), size_(sizeof(T)), ptr_(new T()) {  }

  uint32_t DataSize() const {
    return sizeof(Vector) + num_ * size_;
  }

  uint32_t Serialize(void* buf) const {
    Vector* p = (Vector*)buf;
    p->num_ = num_;
    p->size_ = size_;
    p->ptr_ = (char*)this + sizeof(Vector);
    memcpy(p->ptr_, ptr_, num_ * size_);

    return DataSize();
  }

 private:
  uint32_t num_;
  uint32_t size_;
  T* ptr_;
};

template <typename T>
class Vector<StructPointer<T>> {
 public:
  explicit Vector(uint32_t num) : num_(num), size_(sizeof(StructPointer<T>)), ptr_(new StructPointer<T>()) {  }

  uint32_t DataSize() const {
    return sizeof(Vector) + num_ * size_;
  }

  uint32_t Serialize(void* buf) const {
    Vector* p = (Vector*)buf;
    p->num_ = num_;
    p->size_ = size_;
    p->ptr_ = (char*)this + sizeof(Vector);
    memcpy(p->ptr, ptr_, num_ * size_);

    uint32_t data_size = 0;
    for (size_t i = 0; i < num_; ++i) {
      
    }

    return data_size;
  }

 private:
  uint32_t num_;
  uint32_t size_;
  StructPointer<T>* ptr_;
};
#endif  // STRUCT_POINTER_H
