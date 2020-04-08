#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/Dynamic/VariantValue.h>

#include "func_temp_arg_type_finder.h"

using namespace clang::ast_matchers;
using namespace clang::ast_matchers::dynamic;

namespace {

struct FuncTempArgTypeResult : MatchFinder::MatchCallback {
  FuncTempArgTypeResult(std::set<std::string>& r)
      : result(r) {  }

  void run(const MatchFinder::MatchResult& r) override {
    const auto* decl = r.Nodes.getNodeAs<clang::TemplateArgument>("matched");
    result.insert(decl->getAsType().getAsString());
  }

  std::set<std::string>& result;
};

} // namespace

std::set<std::string> FuncTempArgTypeFinder::Run(
      clang::ASTContext& ctx) {
  std::set<std::string> result;
  FuncTempArgTypeResult typeresult(result);

  DeclarationMatcher match = functionDecl(anyOf(hasName("publish"),
                                                hasName("subscribe")),
        hasTemplateArgument(0, templateArgument().bind("matched")));

  MatchFinder finder;
  finder.addMatcher(match, &typeresult);
  finder.matchAST(ctx);

  return result;
}
