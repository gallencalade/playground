#include <string>

struct Person {
  std::string name;
  void greet();

  Person();
  ~Person();

  class PersonImpl;
  PersonImpl *impl;
};
