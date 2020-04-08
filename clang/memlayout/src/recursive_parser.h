#ifndef MEMLAYOUT_SRC_RECURSIVE_PARSER_H
#define MEMLAYOUT_SRC_RECURSIVE_PARSER_H

#include <map>
#include <string>
#include <vector>

enum TYPECLASS {
  TC_UNKNOWN = 0,
  TC_UNION,
  TC_STRUCT,
  TC_FUNDAMENTAL,
  TC_ENUM,
};

struct FieldLayout {
  std::string name;   // variable name
  std::string type;   // type name: int, char, classA, structB, ...
  TYPECLASS tycls;
  uint32_t offset;
  std::vector<uint32_t> dim;
};

struct RecordLayout {
  std::string name;   // classA, structB, unionC, ...
  TYPECLASS tycls;
  std::vector<FieldLayout> fields;
};

class RecursiveParser {
 public:

 private:
  std::map<std::string, RecordLayout> record_layout_;
};

#endif  // MEMLAYOUT_SRC_RECURSIVE_PARSER_H
