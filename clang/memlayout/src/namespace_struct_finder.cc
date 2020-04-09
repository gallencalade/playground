#include <clang/AST/RecordLayout.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/Dynamic/Parser.h>
#include <clang/ASTMatchers/Dynamic/VariantValue.h>

#include "namespace_struct_finder.h"
#include "utils.h"

#include <iostream>

using namespace clang::ast_matchers;
using namespace clang::ast_matchers::dynamic;

namespace {

struct NamespaceStructResult : MatchFinder::MatchCallback {
  NamespaceStructResult(std::vector<BoundNodes>& r)
      : result(r) {  }

  void run(const MatchFinder::MatchResult& r) override {
    result.push_back(r.Nodes);
  }

  std::vector<BoundNodes>& result;
};

} // namespace

std::vector<BoundNodes> NamespaceStructFinder::Run(clang::ASTContext& ctx,
                                                   const std::string& st) {
  std::string s(st);
  auto ns = split_namespace_struct(s);

  std::string matchstr;
  for (const auto& a : ns) {
    matchstr.append("namedDecl(hasName('" + a + "'), has(");
  }
  matchstr.append("cxxRecordDecl(hasName('" + s +"'),"
        "anyOf(isStruct(), isUnion()), hasDefinition()).bind('matched')");

  matchstr.append(ns.size() * 2, ')');  // two `(` in each namespace decl str

  Diagnostics Diag;
  llvm::Optional<DynTypedMatcher> matcher = Parser::parseMatcherExpression(
      matchstr, &Diag);

  std::vector<BoundNodes> bounds;
  NamespaceStructResult result(bounds);

  MatchFinder finder;
  if (!finder.addDynamicMatcher(*matcher, &result)) {
    llvm::errs() << "Failed to add dynamic matcher\n";
  }

  finder.matchAST(ctx);

  std::cout << bounds.size() << std::endl;

  return bounds;
}
