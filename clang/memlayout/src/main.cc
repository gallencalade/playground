#include <iostream>
#include <fstream>

#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>

#include "memlayout.h"
#include "dumpfile.h"

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

int dump_to_file(const std::map<std::string, RecordLayout>& l) {
  std::ofstream ofs("mapped_layout.h", std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open mapped_layout.h" << std::endl;
    return -1;
  }

  ofs << DUMP_FILE_HEAD;
  for (const auto& [k, v] : l) {
    ofs << "  { \"" << k << "\",\n    " << to_string(v) << "\n  },\n";
  }
  ofs << DUMP_FILE_TAIL;

  return 0;
}

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
  }

  return dump_to_file(layouts);
}
