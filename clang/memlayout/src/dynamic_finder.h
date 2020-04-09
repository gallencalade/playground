#ifndef MEMLAYOUT_SRC_DYNAMIC_FINDER_H
#define MEMLAYOUT_SRC_DYNAMIC_FINDER_H

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/Dynamic/Parser.h>
#include <clang/Frontend/ASTUnit.h>

namespace {

using namespace clang::ast_matchers;

struct DynamicFinderResult : MatchFinder::MatchCallback {
  DynamicFinderResult(std::vector<BoundNodes>& r)
      : result(r) {  }

  void run(const MatchFinder::MatchResult& r) override {
    result.push_back(r.Nodes);
  }

  std::vector<BoundNodes>& result;
};

std::vector<std::string> split_scope_name(std::string& s) {
  std::vector<std::string> tokens;
  size_t pos;
  static const std::string d = "::";
  while ((pos = s.find(d)) != std::string::npos) {
    tokens.emplace_back(s.substr(0, pos));
    s.erase(0, pos + d.length());
  }

  return tokens;
}

} // namespace

class DynamicFinder {
 public:
  template <typename Fn>
  static std::vector<clang::ast_matchers::BoundNodes> Run(
        clang::ASTContext& ctx, const std::string& st) {
    using namespace clang::ast_matchers;
    using namespace clang::ast_matchers::dynamic;

    std::string matchstr = Fn(st);

    Diagnostics Diag;
    llvm::Optional<DynTypedMatcher> matcher = Parser::parseMatcherExpression(
        matchstr, &Diag);

    std::vector<BoundNodes> bounds;
    DynamicFinderResult result(bounds);

    MatchFinder finder;
    if (!finder.addDynamicMatcher(*matcher, &result)) {
      llvm::errs() << "Failed to add dynamic matcher\n";
      return std::vector<BoundNodes>();
    }

    finder.matchAST(ctx);

    return bounds;
  }

  static std::string RecordFinderStr(std::string s) {
    auto ns = split_scope_name(s);

    std::string matchstr;
    for (const auto& a : ns) {
      matchstr.append("namedDecl(hasName('" + a + "'), has(");
    }
    matchstr.append("cxxRecordDecl(hasName('" + s +"'),"
          "anyOf(isStruct(), isUnion()), hasDefinition()).bind('matched')");
    matchstr.append(ns.size() * 2, ')');  // two `(` in each namespace decl str

    return matchstr;
  }

  static std::string EnumFinderStr(std::string s) {
    auto ns = split_scope_name(s);

    std::string matchstr;
    for (const auto& a : ns) {
      matchstr.append("namedDecl(hasName('" + a + "'), has(");
    }
    matchstr.append("enumDecl(hasName('" + s +"'), "
                    "hasDefinition()).bind('matched')");
    matchstr.append(ns.size() * 2, ')');  // two `(` in each namespace decl str

    return matchstr;
  }
};

#endif  // MEMLAYOUT_SRC_DYNAMIC_FINDER_H
