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
  int32_t offset;    // number for enum, but offset for others
  std::vector<int32_t> dim;    // 1 as default
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

inline std::string to_string(TYPECLASS t) {
  switch (t) {
  case TYPECLASS::TC_UNION:
    return "TYPECLASS::TC_UNION";
  case TYPECLASS::TC_STRUCT:
    return "TYPECLASS::TC_STRUCT";
  case TYPECLASS::TC_FUNDAMENTAL:
    return "TYPECLASS::TC_FUNDAMENTAL";
  case TYPECLASS::TC_ENUM:
    return "TYPECLASS::TC_ENUM";
  default:
    return "TYPECLASS::TC_UNKNOWN";
  }
}

inline std::ostream& operator<<(std::ostream& os, const FieldLayout& f) {
  os << "+ type  : " << f.type << std::endl;
  os << "  var   : " << f.var << std::endl;
  os << "  tycls : " << to_string(f.tycls) << std::endl;
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
  os << "tycls  : " << to_string(r.tycls) << std::endl;
  os << "fields : " << std::endl;
  for (const auto& a : r.fields) {
    os << a << std::endl;
  }

  return os;
}

inline std::string to_string(const FieldLayout& f) {
  std::string s("{ \"" + f.type + "\", \"" + f.var + "\", " +
                to_string(f.tycls) + ", " + std::to_string(f.offset) + ", { ");
  for (size_t i = 0; i < f.dim.size(); ++i) {
    s.append(std::to_string(f.dim[i]));
    s.append((i == f.dim.size() - 1) ? "" : ", ");
  }
  s.append(" } }");

  return s;
}

inline std::string to_string(const RecordLayout& r) {
  std::string s("{ \"" + r.name + "\", " + to_string(r.tycls) + ", { ");
  for (size_t i = 0; i < r.fields.size(); ++i) {
    s.append(to_string(r.fields[i]));
    s.append((i == r.fields.size() - 1) ? "" : ", ");
  }
  s.append(" } };");

  return s;
}

#endif  // STRUCTLAYOUT_SRC_RECORD_LAYOUT_H_
