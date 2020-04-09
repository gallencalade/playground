#ifndef MEMLAYOUT_SRC_MEMLAYOUT_H
#define MEMLAYOUT_SRC_MEMLAYOUT_H

#include <memory>
#include <vector>

#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/Tooling.h>

#include "record_layout.h"
#include "func_temp_arg_type_finder.h"
#include "namespace_struct_finder.h"

class MemLayout {
 public:
  MemLayout(const clang::tooling::CompilationDatabase& compdb,
            const std::vector<std::string>& files);

  int ParseRecordLayout(const std::string& name,
        const clang::CXXRecordDecl* decl);

  int FindNamedRecord(const std::string& name);


  static TYPECLASS TypeCheck(const clang::Type* t);

  FieldLayout ParseField(const clang::FieldDecl* decl, uint32_t offset);

  std::map<std::string, std::vector<StructField>> Run();

 private:
/*  int RecursiveParse(const std::string& st,
      std::map<std::string, std::vector<StructField>>& mapper);
*/
  std::set<std::string> FuncTempArgTypeQuery();

  std::vector<StructField> NamespaceStructLayoutQuery(const std::string& st);

 private:
  std::map<std::string, RecordLayout> record_layout_;

  clang::tooling::ClangTool tool_;
  std::vector<std::unique_ptr<clang::ASTUnit>> asts_;
};

#endif  // MEMLAYOUT_SRC_MEMLAYOUT_H
