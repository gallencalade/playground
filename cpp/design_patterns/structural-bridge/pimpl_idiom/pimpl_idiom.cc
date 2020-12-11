#include <iostream>

#include "pimpl_idiom.h"

struct Person::PersonImpl {
  void greet(Person* p);
};

void Person::PersonImpl::greet(Person* p) {
  std::cout << "hello " << p->name << std::endl;
}

Person::Person()
  : impl(new PersonImpl) { }

Person::~Person() { delete impl; }

void Person::greet() {
  impl->greet(this);
}
