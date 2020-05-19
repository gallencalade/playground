#include <cstring>
#include <iostream>
#include <vector>

class StructPointer {
 public:
  StructPointer() = default;

  StructPointer(const StructPointer&) = delete;

  StructPointer(StructPointer&& sp) {
    if (this != &sp) {
      ptr_ = sp.ptr_;
      pdata_ = sp.pdata_;
      sp.ptr_ = nullptr;
    }
  }

  StructPointer& operator=(const StructPointer&) = delete;

  ~StructPointer()  = default;

  template <typename T>
  void InitAs() {
    ptr_ = new T();
  }

  template <typename T>
  void UninitAs() {
    if (nullptr != ptr_) {
      T* p = (T*)ptr_;
      delete p;
      ptr_ = nullptr;
    }
  }

  void Init(const unsigned char* p) {
    pdata_ = p;
  }

  template <typename T>
  T* GetAs() {
    if (nullptr == ptr_ && nullptr != pdata_) {
      ptr_ = new T(pdata_);
    }
    return (T*)ptr_;
  }

  template <typename T>
  void SetAs(const T* p) { memcpy(ptr_, p, sizeof(T)); }    // shallow copy ??

  template <typename T>
  void SetAs(const T& p) { memcpy(ptr_, &p, sizeof(T)); }   // shallow copy ??

  bool IsNull() const { return nullptr == ptr_; }

 private:
  const unsigned char* pdata_ = nullptr;
  void* ptr_ = nullptr;
};


class Street {
 public:
  Street() : writable_(true),
             data_size_(sizeof(uint64_t)),           // << This is sizeof(a)
             data_(new unsigned char[data_size_]) { }

  explicit Street(const unsigned char* buf) : writable_(false) {
    if (nullptr == buf) {
      throw std::runtime_error("Nullptr buffer!");
    }
    // deserization
    const unsigned char* p = buf;
    // uint64_t struct_size = *(uint64_t*)p;
    p += sizeof(uint64_t);

    data_size_ = *(uint64_t*)p;
    p += sizeof(uint64_t);

    // uint64_t num_pointers = *(uint64_t*)p;
    p += sizeof(uint64_t);
    // sets data fields, data pointer
    data_ = const_cast<unsigned char*>(p);
    // p += data_size_;
  }

  Street(const Street&) = delete;

  Street(Street&&) = delete;

  Street& operator=(const Street&) = delete;

  ~Street() {
    if (writable_) {
      delete[] data_;
      data_ = nullptr;
      data_size_ = 0;
    }
  }

  bool has_a() const { return (0 < data_size_ ? true : false); }   // << 0 is offset of a

  uint64_t get_a() { return *(uint64_t*)(data_ + 0); }             // << 0 is offset of a

  void set_a(uint64_t l) { *(uint64_t*)(data_ + 0) = l; }          // << 0 is offset of a

 private:
  // uint64_t a;
  bool writable_;   // < from new or from buffer

  // data fields
  uint64_t data_size_ = 0;
  unsigned char* data_ = nullptr;
};


class House {
 public:
  House() : writable_(true),
            data_size_(sizeof(uint64_t)),           // << This is sizeof(location)
            data_(new unsigned char[data_size_]),
            pointers_(1) {
    pointers_[0].InitAs<Street>();
  }

  explicit House(const unsigned char* buf) : writable_(false) {
    if (nullptr == buf) {
      throw std::runtime_error("Nullptr buffer!");
    }
    // deserization
    const unsigned char* p = buf;
    // uint64_t struct_size = *(uint64_t*)p;
    p += sizeof(uint64_t);

    data_size_ = *(uint64_t*)p;
    p += sizeof(uint64_t);

    uint64_t num_pointers = *(uint64_t*)p;
    p += sizeof(uint64_t);
    // sets data fields, data pointer
    data_ = const_cast<unsigned char*>(p);
    p += data_size_;

    // sets pointer fields, struct pointers
    pointers_.resize(num_pointers);
    for (size_t i = 0; i < num_pointers; ++i) {
      pointers_[i].Init(p);
      uint64_t struct_size = *(uint64_t*)p;
      p += struct_size;
    }
  }

  House(const House&) = delete;

  House(House&&) = delete;

  House& operator=(const House&) = delete;

  ~House() {
    if (writable_) {
      delete[] data_;
      data_ = nullptr;
      data_size_ = 0;
    }
    pointers_[0].UninitAs<Street>();
  }

  bool has_location() const { return (0 < data_size_ ? true : false); }   // << 0 is offset of a

  uint64_t get_location() { return *(uint64_t*)(data_ + 0); }             // << 0 is offset of a

  void set_location(uint64_t l) { *(uint64_t*)(data_ + 0) = l; }          // << 0 is offset of a

  bool has_street() const { return (0 < pointers_.size()); }

  Street* get_street() {
    return (0 < pointers_.size() ? pointers_[0].GetAs<Street>() : nullptr);
  }// Should not be writable

  void set_street(const Street& h) { if (has_street()) pointers_[0].SetAs<Street>(h); }

 private:
  // uint64_t location;
  // Street street;
  bool writable_;   // < from new or from buffer

  // data fields
  uint64_t data_size_ = 0;
  unsigned char* data_ = nullptr;

  std::vector<StructPointer> pointers_;
};


class Person {
 public:
  Person() : writable_(true),
             data_size_(sizeof(uint64_t)),   // sizeof(a)
             data_(new unsigned char[data_size_]),
             pointers_(2) {
    pointers_[0].InitAs<House>();
    pointers_[1].InitAs<House>();
  }

  explicit Person(const unsigned char* buf) : writable_(false) {
    if (nullptr == buf) {
      throw std::runtime_error("Nullptr buffer!");
    }
    // deserization
    const unsigned char* p = buf;
    // uint64_t struct_size = *(uint64_t*)p;
    p += sizeof(uint64_t);    // all struct length

    data_size_ = *(uint64_t*)p;
    p += sizeof(uint64_t);    // data size in this struct

    uint64_t num_pointers = *(uint64_t*)p;
    p += sizeof(uint64_t);    // how many pointers
    // sets data fields, data pointer
    data_ = const_cast<unsigned char*>(p);
    p += data_size_;          // jump data fields

    // sets pointer fields, struct pointers
    pointers_.resize(num_pointers);
    for (size_t i = 0; i < num_pointers; ++i) {
      pointers_[i].Init(p);
      uint64_t struct_size = *(uint64_t*)p;
      p += struct_size;
    }
  }

  Person(const Person&) = delete;

  Person(Person&&) = delete;

  Person& operator=(const Person&) = delete;

  ~Person() {
    if (writable_) {
      delete[] data_;
      data_ = nullptr;
      data_size_ = 0;
    }
    pointers_[0].UninitAs<House>();
    pointers_[1].UninitAs<House>();
  }

 public:
  bool has_id() const { return (0 < data_size_ ? true : false); }   // << offset of a is 0

  uint64_t get_id() const {
    return (has_id() ? *(uint64_t*)(data_ + 0) : 0 /* or default value */);
  }

  void set_id(uint64_t i) { if (has_id()) *(uint64_t*)(data_ + 0) = i; }          // << offset of a is 0

  bool has_house1() const { return (0 < pointers_.size()); }

  House* get_house1() {
    return (0 < pointers_.size() ? pointers_[0].GetAs<House>() : nullptr);
  }// Should not be writable

  void set_house1(const House& h) { if (has_house1()) pointers_[0].SetAs<House>(h); }

  bool has_house2() const { return (1 < pointers_.size()); }

  House* get_house2() {
    return (0 < pointers_.size() ? pointers_[1].GetAs<House>() : nullptr);
  }// Should not be writable

  void set_house2(const House& h) { if (has_house2()) pointers_[1].SetAs<House>(h); }

 private:
  // uint64_t id;
  // struct House house;
  bool writable_;   // from new or from buffer

  uint64_t data_size_ = 0;
  unsigned char* data_ = nullptr;

  std::vector<StructPointer> pointers_;
};


int main() {
  Person ps;
  ps.set_id(10);
  auto* h = ps.get_house1();
  h->set_location(123);

  unsigned char buf[sizeof(uint64_t) * 20] = { 0 };
  uint64_t* ptr = (uint64_t*)buf;
  ptr[0] = sizeof(uint64_t) * 20; // struct_size + data size + num pointers + sizeof(id) + .....
  ptr[1] = sizeof(uint64_t);  // id
  ptr[2] = 2;
  ptr[3] = 10;
  ptr[4] = sizeof(uint64_t) * 8;  // struct_size + data size + num pointers + sizeof()
  ptr[5] = sizeof(uint64_t);  // location
  ptr[6] = 1;
  ptr[7] = 123;
  ptr[8] = sizeof(uint64_t) * 4;
  ptr[9] = sizeof(uint64_t);  // a
  ptr[10] = 0;
  ptr[11] = 888;
  ptr[12] = sizeof(uint64_t) * 8;  // struct_size + data size + num pointers + sizeof()
  ptr[13] = sizeof(uint64_t);  // location
  ptr[14] = 1;
  ptr[15] = 321;
  ptr[16] = sizeof(uint64_t) * 4;
  ptr[17] = sizeof(uint64_t);  // a
  ptr[18] = 0;
  ptr[19] = 999;

  Person pd(buf);
  std::cout << pd.get_id() << std::endl;
  std::cout << pd.has_house1() << std::endl;
  std::cout << pd.get_house1()->get_location() << std::endl;
  std::cout << pd.get_house1()->get_street()->get_a() << std::endl;
  std::cout << pd.has_house2() << std::endl;
  std::cout << pd.get_house2()->get_location() << std::endl;
  std::cout << pd.get_house2()->get_street()->get_a() << std::endl;

  return 0;
}
