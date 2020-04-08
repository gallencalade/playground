#ifndef STRUCT_H_
#define STRUCT_H_

#include <vector>
#include <string>

struct S {
  std::string name;
  std::vector<S> s;
};

#endif  STRUCT_H_
