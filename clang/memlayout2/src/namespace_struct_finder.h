#ifndef MEMLAYOUT_SRC_NAMESPACE_STRUCT_FUNDER_H
#define MEMLAYOUT_SRC_NAMESPACE_STRUCT_FUNDER_H

#include <clang/Frontend/ASTUnit.h>
#include <clang/ASTMatchers/ASTMatchers.h>

#include "struct_layout.h"

class NamespaceStructFinder {
 public:
  static std::vector<StructField> Run(clang::ASTContext& ctx,
                                      const std::string& st);

 private:
  static std::vector<StructField> GetStructFields(
        const clang::ast_matchers::BoundNodes& bn);
};

#endif  // MEMLAYOUT_SRC_NAMESPACE_STRUCT_FUNDER_H
