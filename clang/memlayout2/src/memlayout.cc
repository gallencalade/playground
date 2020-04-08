#include "memlayout.h"
#include "utils.h"

MemLayout::MemLayout(const clang::tooling::CompilationDatabase& compdb,
                     const std::vector<std::string>& files)
    : tool_(compdb, files) {
  if (tool_.buildASTs(asts_)) {
    throw std::runtime_error("Failed to build ASTs");
  }
}


std::map<std::string, std::vector<StructField>> MemLayout::Run() {
  std::map<std::string, std::vector<StructField>> layouts;
  auto struct_names = FuncTempArgTypeQuery();
  for (const auto& a : struct_names) {
    if (0 != RecursiveParse(remove_prefix(a), layouts)) {
      return std::map<std::string, std::vector<StructField>>();
    }
  }

  return layouts;
}

int MemLayout::RecursiveParse(const std::string& st,
      std::map<std::string, std::vector<StructField>>& mapper) {
  if (mapper.end() != mapper.find(st)) {
    return 0;
  }

  auto l = NamespaceStructLayoutQuery(st);
  if (l.empty()) {
    return -1;
  }

  mapper[st] = l;

  for (const auto& a : l) {
    if (!a.builtin && (mapper.end() == mapper.find(a.type))) {
      if (0 != RecursiveParse(a.type, mapper)) {
        return -1;
      }
    }
  }

  return 0;
}

std::vector<StructField> MemLayout::NamespaceStructLayoutQuery(
      const std::string& st) {
  std::vector<StructField> result;
  for (auto& a : asts_) {
    result = NamespaceStructFinder::Run(a->getASTContext(), st);
    if (!result.empty()) {
      return result;
    }
  }

  llvm::errs() << "No definition found: " << st << "\n";

  return result;
}

std::set<std::string> MemLayout::FuncTempArgTypeQuery() {
  std::set<std::string> result;
  for (auto& a : asts_) {
    auto r = FuncTempArgTypeFinder::Run(a->getASTContext());
    result.merge(r);
  }

  return result;
}
