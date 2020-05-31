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

  StructPointer(const StructPointer& other) : own_(true), ptr_(new T) {
    *ptr_ = *other.ptr_;
  }

  StructPointer& operator=(StructPointer other) {
    Swap(*this, other);
    return *this;
  }

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
  friend void Swap(StructPointer& lhs, StructPointer& rhs) {
    // std::swap(lhs.own_, rhs.own_);
    std::swap(lhs.ptr_, rhs.ptr_);
  }

 private:
  bool own_;
  T* ptr_;
};

template <typename T>
class alignas(8) Vector {
 public:
  explicit Vector()
    : own_(true), cap_(4), num_(0), ptr_(new T[cap_]) {  }

  explicit Vector(std::initializer_list<T> t)
    : own_(true), cap_(t.size()), num_(0), ptr_(new T[cap_]) {
      for (auto a : t) {
        PushBack(a);
      }
    }

  explicit Vector(uint32_t num)
    : own_(true), cap_(num), num_(num), ptr_(cap_ ? new T[cap_] : nullptr) {  }

  Vector(Vector& other)
    : own_(true), cap_(other.cap_), num_(other.num_), ptr_(cap_ ? new T[cap_] : nullptr) {
    std::memcpy(ptr_, other.ptr_, num_ * sizeof(T));
  }

  Vector& operator=(Vector other) {
    Swap(*this, other);
    return *this;
  }

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

  void PushBack(const T& t) {
    if (num_ == cap_) {
      Reallocate();
    }

    ptr_[num_++] = t;
  }

 private:
  void Reallocate() {
    static const uint32_t extsize = 4;
    cap_ += extsize;
    T* p = new T[cap_];
    std::memcpy(p, ptr_, num_ * sizeof(T));
    delete[] ptr_;
    ptr_ = p;
  }

  friend void Swap(Vector& lhs, Vector& rhs) {
    // std::swap(lhs.own_, rhs.own_);
    std::swap(lhs.cap_, rhs.cap_);
    std::swap(lhs.num_, rhs.num_);
    std::swap(lhs.ptr_, rhs.ptr_);
  }

 private:
  bool own_;
  uint32_t cap_;
  uint32_t num_;
  T* ptr_;
};

template <typename T>
class alignas(8) Vector<StructPointer<T>> {
 public:
  explicit Vector()
    : own_(true), cap_(4), num_(0), ptr_(new StructPointer<T>[cap_]) {  }

  explicit Vector(std::initializer_list<T> t)
    : own_(true), cap_(t.size()), num_(0), ptr_(new StructPointer<T>[cap_]) {
      for (auto a : t) {
        PushBack(a);
      }
    }

  explicit Vector(uint32_t num)
    : own_(true), cap_(num), num_(num), ptr_(num ? new StructPointer<T>[num_] : nullptr) {  }

  Vector(Vector& other)
    : own_(true), cap_(other.cap_), num_(other.num_), ptr_(cap_ ? new StructPointer<T>[cap_] : nullptr) {
    for (uint32_t i = 0; i < other.num_; ++i) {
      ptr_[i] = other.ptr_[i];
    }
  }

  Vector& operator=(Vector other) {
    Swap(*this, other);
    return *this;
  }

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
    std::memcpy((void*)p, this, sizeof(Vector));
    p->own_ = false;
    p->ptr_ = reinterpret_cast<StructPointer<T>*>(sizeof(Vector));

    uint32_t data_size = 0;
    for (size_t i = 0; i < num_; ++i) {
      data_size += ptr_[i].Serialize((char*)buf + (uintptr_t)p->ptr_ + i * sizeof(StructPointer<T>),
            data_size + (num_ - i) * sizeof(StructPointer<T>));
    }

    return data_size + sizeof(Vector) + num_ * sizeof(StructPointer<T>);
  }

  T& operator[](uint32_t idx) {
    if (own_) {
      return ptr_[idx].Ref();
    } else {
      return ((StructPointer<T>*)((char*)this + (uintptr_t)ptr_))[idx].Ref();
    }
  }

  uint32_t Num() const { return num_; }

  void PushBack(const T& t) {
    if (num_ == cap_) {
      Reallocate();
    }

    ptr_[num_++] = StructPointer<T>(t);
  }

 private:
  void Reallocate() {
    static const uint32_t extsize = 4;
    cap_ += extsize;
    StructPointer<T>* p = new StructPointer<T>[cap_];
    std::memcpy(p, ptr_, num_ * sizeof(StructPointer<T>));
    delete[] ptr_;
    ptr_ = p;
  }

  friend void Swap(Vector& lhs, Vector& rhs) {
    // std::swap(lhs.own_, rhs.own_);
    std::swap(lhs.cap_, rhs.cap_);
    std::swap(lhs.num_, rhs.num_);
    std::swap(lhs.ptr_, rhs.ptr_);
  }

 private:
  bool own_;
  uint32_t cap_;
  uint32_t num_;
  StructPointer<T>* ptr_;
};

#endif  // STRUCT_POINTER_H
