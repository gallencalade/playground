#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/Dynamic/VariantValue.h>

#include "named_enum_finder.h"

using namespace clang::ast_matchers;
using namespace clang::ast_matchers::dynamic;

namespace {

struct NamedEnumResult : MatchFinder::MatchCallback {
  NamedEnumResult(std::set<std::string>& r)
      : result(r) {  }

  void run(const MatchFinder::MatchResult& r) override {
    const auto* decl = r.Nodes.getNodeAs<clang::EnumDecl>("matched");
    result.insert(decl->getAsType().getAsString());
  }

  std::set<std::string>& result;
};

} // namespace

std::set<std::string> NamedEnumFinder::Run(
      clang::ASTContext& ctx) {
  std::set<std::string> result;
  NamedEnumResult typeresult(result);

  DeclarationMatcher match = functionDecl(anyOf(hasName("publish"),
                                                hasName("subscribe")),
        hasTemplateArgument(0, templateArgument().bind("matched")));

  MatchFinder finder;
  finder.addMatcher(match, &typeresult);
  finder.matchAST(ctx);

  return result;
}
