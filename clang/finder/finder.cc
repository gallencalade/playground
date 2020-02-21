#include <iostream>
#include <fstream>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/Tooling.h>

using namespace llvm;
using namespace clang;
using namespace clang::diag;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class DumpCheck : public MatchFinder::MatchCallback {
  virtual void run(const MatchFinder::MatchResult& result) {
    static int cnt = 0;
    const auto* decl = result.Nodes.getNodeAs<CXXRecordDecl>("matched");
    std::cout << ">>>> MATCHED No." << ++cnt << std::endl;
    decl->dump();
  }
};

int main(int argc, char* argv[]) {
  if (2 != argc) {
    std::cerr << "Unspcified cpp file" << std::endl;
    return -1;
  }


  std::ifstream ifs(argv[1], std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open cpp file" << std::endl;
    return -1;
  }

  std::string content((std::istreambuf_iterator<char>(ifs)),
                      std::istreambuf_iterator<char>());

  DumpCheck checker;
  MatchFinder finder;
  finder.addMatcher(recordDecl().bind("matched"), &checker);

  std::unique_ptr<FrontendActionFactory> factory(newFrontendActionFactory(&finder));

  if (!clang::tooling::runToolOnCode(factory->create(), content)) {
    return -1;
  }

  return 0;
}
