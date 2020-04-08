#ifndef MEMLAYOUT_SRC_NAMED_ENUM_FINDER_H
#define MEMLAYOUT_SRC_NAMED_ENUM_FINDER_H

#include <set>
#include <string>

#include <clang/Frontend/ASTUnit.h>

class NamedEnumFinder {
 public:
  static std::set<std::string> Run(clang::ASTContext& ctx);
};

#endif  // MEMLAYOUT_SRC_NAMED_ENUM_FINDER_H
