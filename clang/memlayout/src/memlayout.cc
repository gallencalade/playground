#include <clang/AST/RecordLayout.h>

#include "memlayout.h"
#include "dynamic_finder.h"
#include "recursive_parser.h"

MemLayout::MemLayout(const clang::tooling::CompilationDatabase& compdb,
                     const std::vector<std::string>& files)
    : tool_(compdb, files) {
  if (tool_.buildASTs(asts_)) {
    throw std::runtime_error("Failed to build ASTs");
  }
}

MemLayout::MemLayoutMap MemLayout::Run(const std::vector<std::string>& funcname) {
  MemLayout::MemLayoutMap result;
  for (const auto& a : funcname) {
    auto temp_names = FuncTempArgTypeQuery(a);
    for (const auto& b : temp_names) {
      if (0 != RecordLayoutQuery(b, result)) {
        return MemLayout::MemLayoutMap();
      }
    }
  }

  return result;
}

int MemLayout::RecordLayoutQuery(const std::string& name,
                                 MemLayout::MemLayoutMap& lm) {
  int status = -1;
  for (auto& a : asts_) {
    RecursiveParser parser(a->getASTContext(), lm);
    if (-1 == (status = parser.ParseNamedRecord(name))) {
      return -1;
    }
  }

  if (1 == status) {
    llvm::errs() << "Failed to find " << name << " difinition\n";
    return -1;
  }

  return 0;
}

std::set<std::string> MemLayout::FuncTempArgTypeQuery(const std::string& fn) {
  std::set<std::string> result;
  for (auto& a : asts_) {
    auto ret = DynamicFinder::Find<DynamicFinder::FuncTempArgTypeFinderStr>(
          a->getASTContext(), fn);
    for (const auto& b : ret) {
      const auto* decl = b.getNodeAs<clang::TemplateArgument>("matched");
      result.insert(decl->getAsType().getAsString());
    }
  }

  return result;
}
