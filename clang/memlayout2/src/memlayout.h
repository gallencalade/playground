#ifndef MEMLAYOUT_SRC_MEMLAYOUT_H
#define MEMLAYOUT_SRC_MEMLAYOUT_H

#include <memory>
#include <vector>

#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/Tooling.h>

#include "func_temp_arg_type_finder.h"
#include "namespace_struct_finder.h"

class MemLayout {
 public:
  MemLayout(const clang::tooling::CompilationDatabase& compdb,
            const std::vector<std::string>& files);

  std::map<std::string, std::vector<StructField>> Run();

 private:
  int RecursiveParse(const std::string& st,
      std::map<std::string, std::vector<StructField>>& mapper);

  std::set<std::string> FuncTempArgTypeQuery();

  std::vector<StructField> NamespaceStructLayoutQuery(const std::string& st);

 private:
  clang::tooling::ClangTool tool_;
  std::vector<std::unique_ptr<clang::ASTUnit>> asts_;
};

#endif  // MEMLAYOUT_SRC_MEMLAYOUT_H
