#ifndef MEMLAYOUT_SRC_STRUCT_LAYOUT_H_
#define MEMLAYOUT_SRC_STRUCT_LAYOUT_H_

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

struct StructField {
  std::string name;             // < variable name
  std::string type;             // < type name
  bool builtin;                 // < is builtin type
  uint64_t offset;              // < offset
  std::vector<uint32_t> dim;    // < dimention
};

inline std::ostream& operator<<(std::ostream& os, const StructField& sf) {
  os << "name     : " << sf.name << std::endl;
  os << "type     : " << sf.type << std::endl;
  os << "builtin  : " << sf.builtin << std::endl;
  os << "offset   : " << sf.offset << std::endl;
  os << "dim      : ";
  for (const auto& a : sf.dim) {
    os << a << " ";
  }

  return os;
}

// struct StructLayout {
//   std::string type;             // < type name
//   std::vector<std::string> ns;  // < namespace type in
//   std::vector<StructField> field;   // < items
// };

#endif  // STRUCTLAYOUT_SRC_STRUCT_LAYOUT_H_
