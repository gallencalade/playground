#include <iostream>

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>

#include "memlayout.h"

static llvm::cl::OptionCategory MemLayoutOptCategory("memlayout options");

/*static llvm::cl::opt<std::string> Entrance("entrfile",
    llvm::cl::desc("A file including the method entrance."),
    llvm::cl::init(""),
    llvm::cl::cat(MemLayoutOptCategory));

static llvm::cl::opt<std::string> Func("func",
    llvm::cl::desc("Function name in entrfile, 'main' as default."),
    llvm::cl::init("main"),
    llvm::cl::cat(MemLayoutOptCategory));*/

static llvm::cl::extrahelp CommonHelp(
      clang::tooling::CommonOptionsParser::HelpMessage);

int main(int argc, const char** argv) {
  clang::tooling::CommonOptionsParser optionsParser(argc, argv,
                                                    MemLayoutOptCategory);
  const auto& compilations = optionsParser.getCompilations();
  MemLayout memlayout(compilations, compilations.getAllFiles());
                                 // optionsParser.getSourcePathList());

  const std::vector<std::string> func_names{ "publish", "subscribe" };
  auto layouts = memlayout.Run(func_names);
  for (const auto& [k, v] : layouts) {
    std::cout << "KEY  : " << k << std::endl;
    std::cout << "VALUE: " << v << std::endl;
    std::cout << to_string(v) << std::endl;
  }

  return 0;
}
