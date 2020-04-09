#ifndef MEMLAYOUT_SRC_RECORD_FINDER_H
#define MEMLAYOUT_SRC_RECORD_FINDER_H

#include <clang/Frontend/ASTUnit.h>
#include <clang/ASTMatchers/ASTMatchers.h>

#include "struct_layout.h"

class NamespaceStructFinder {
 public:
  static std::vector<clang::ast_matchers::BoundNodes> Run(
        clang::ASTContext& ctx, const std::string& st);

 private:
  static std::string RecordFinderStr(std::string s);
  static std::string EnumFinderStr(std::string s);
};

#endif  // MEMLAYOUT_SRC_RECORD_FINDER_H
