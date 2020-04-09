#ifndef MEMLAYOUT_SRC_MEMLAYOUT_H
#define MEMLAYOUT_SRC_MEMLAYOUT_H

#include <memory>
#include <vector>

#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/Tooling.h>

#include "record_layout.h"

class MemLayout {
  using MemLayoutMap = std::map<std::string, RecordLayout>;

 public:
  MemLayout(const clang::tooling::CompilationDatabase& compdb,
            const std::vector<std::string>& files);

  MemLayoutMap Run(const std::vector<std::string>& funcname);

 private:
  std::set<std::string> FuncTempArgTypeQuery(const std::string& fn);

  int RecordLayoutQuery(const std::string& name, MemLayoutMap& lm);

 private:
  clang::tooling::ClangTool tool_;
  std::vector<std::unique_ptr<clang::ASTUnit>> asts_;
};

#endif  // MEMLAYOUT_SRC_MEMLAYOUT_H
