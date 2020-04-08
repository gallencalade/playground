#ifndef MEMLAYOUT_SRC_FUNC_TEMP_ARG_TYPE_FINDER_H
#define MEMLAYOUT_SRC_FUNC_TEMP_ARG_TYPE_FINDER_H

#include <set>
#include <string>

#include <clang/Frontend/ASTUnit.h>

class FuncTempArgTypeFinder {
 public:
  static std::set<std::string> Run(clang::ASTContext& ctx);
};

#endif  // MEMLAYOUT_SRC_FUNC_TEMP_ARG_TYPE_FINDER_H
