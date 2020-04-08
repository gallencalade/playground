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

/*  if (1 < bounds.size()) {
    llvm::errs() << "More than one definitions: " << bounds.size() << "\n";
    return std::vector<StructField>();
  }

  if (bounds.empty()) {   // not an error, just not found int this ctx
    // llvm::errs() << "Definition of " << s << "in " << " is not found\n";
    return std::vector<StructField>();
  }

  const auto* decl = bounds.at(0).getNodeAs<clang::CXXRecordDecl>("matched");

  return GetStructFields(bounds[0]);*/
}

/*std::vector<StructField> NamespaceStructFinder::GetStructFields(
      const clang::ast_matchers::BoundNodes& bn) {
  // const auto* td = bn.getNodeAs<clang::TagDecl>("union")->getDefinition();
  // td->dump();

  const auto* decl = bn.getNodeAs<clang::CXXRecordDecl>("matched");

  for (auto* d : decl->decls()) {
    if (d->getKind() == clang::Decl::Kind::CXXRecord) {
      llvm::outs() << static_cast<clang::CXXRecordDecl*>(d)->getNameAsString() << "\n";
    }
    llvm::outs() << d->getDeclKindName() << "\n";
  }
  exit(0);

  const auto& l = decl->getASTContext().getASTRecordLayout(decl);

  std::vector<StructField> result;
  result.reserve(l.getFieldCount());
  for (const auto& f : decl->fields()) {
    // return type
    const auto& dtype = f->getType().getDesugaredType(decl->getASTContext());

    std::string type = ::remove_prefix(dtype.getAsString());
    bool builtin = dtype.getTypePtr()->isFundamentalType();
    std::vector<uint32_t> dim({ 1 });
    // return if is array
    if (dtype.getTypePtr()->isConstantArrayType()) {
      const auto* arr = f->getASTContext().getAsConstantArrayType(dtype);
      type = arr->getElementType().getAsString();
      builtin = arr->getElementType().getTypePtr()->isFundamentalType();
      dim[0] = std::stoi(arr->getSize().toString(10, true));

      while (arr->getElementType().getTypePtr()->isConstantArrayType()) {
        arr = f->getASTContext().getAsConstantArrayType(arr->getElementType());
        type = arr->getElementType().getAsString();
        builtin = arr->getElementType().getTypePtr()->isFundamentalType();
        dim.push_back(std::stoi(arr->getSize().toString(10, true)));
      }
    }

    auto offset = l.getFieldOffset(f->getFieldIndex());
    result.push_back({ f->getNameAsString(), type, builtin, offset,
                       std::move(dim) });
  }

  return result;
}*/