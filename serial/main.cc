#include <iostream>

class Reader {
 public:
  explicit Reader(const char* p) : p_(p) {  }

  template <typename T>
  const T* getAs(int offset) { return (T*)(p_ + offset); }

 private:
  const char* p_;
};

class Builder {
 public:
  explicit Builder(int s) {
    p_ = new char[s];
  }

  ~Builder() { delete[] p_; }

  template <typename T>
  void setAs(int offset, const T& t) {
    *(p_ + offset) = t;
  }

  template <typename T>
  const T* getAs(int offset) { return (T*)(p_ + offset); }

 private:
  char* p_ = nullptr;
};

class C {
 public:

 private:

};

int main() {
  C c;
  c.set_a(10);
  c.set_b(20);
  c.set_c(30);

  std::cout << c.get_a() << std::endl;
  std::cout << c.get_b() << std::endl;
  std::cout << c.get_c() << std::endl;

  return 0;
}
