#ifndef MEMLAYOUT_SRC_RECORD_LAYOUT_H_
#define MEMLAYOUT_SRC_RECORD_LAYOUT_H_

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

enum TYPECLASS {
  TC_UNKNOWN,
  TC_UNION,
  TC_STRUCT,
  TC_FUNDAMENTAL,
  TC_ENUM,
};

struct FieldLayout {
  std::string type;   // type name: int, char, classA, structB, ...
  std::string var;
  TYPECLASS tycls;
  uint32_t offset;
  std::vector<uint32_t> dim;
};

/**
 * \class RecordLayout
 * \brief Including struct/union and enum
 */
struct RecordLayout {
  std::string name;   // classA, structB, unionC, ...
  TYPECLASS tycls;
  std::vector<FieldLayout> fields;
};

inline std::ostream& operator<<(std::ostream& os, const FieldLayout& f) {
  os << "+ type  : " << f.type << std::endl;
  os << "  var   : " << f.var << std::endl;
  os << "  tycls : " << f.tycls << std::endl;
  os << "  offset: " << f.offset << std::endl;
  os << "  dim   : ";
  for (const auto a : f.dim) {
    os << a << " ";
  }

  return os;
}

inline std::ostream& operator<<(std::ostream& os, const RecordLayout& r) {
  os << "---------" << std::endl;
  os << "name   : " << r.name << std::endl;
  os << "tycls  : " << r.tycls << std::endl;
  os << "fields : " << std::endl;
  for (const auto& a : r.fields) {
    os << a << std::endl;
  }

  return os;
}

#endif  // STRUCTLAYOUT_SRC_RECORD_LAYOUT_H_
