#ifndef MEMLAYOUT_SRC_RECURSIVE_PARSER_H
#define MEMLAYOUT_SRC_RECURSIVE_PARSER_H

#include <clang/Frontend/ASTUnit.h>

#include "record_layout.h"

class RecursiveParser {
 public:
  explicit RecursiveParser(clang::ASTContext& ctx,
                           std::map<std::string, RecordLayout>& rl)
      : astctx_(ctx), record_layout_(rl) {  }

  int Parse(const std::string& name);

 private:
  int ParseRecordLayout(const std::string& name,
                        const clang::CXXRecordDecl* decl);

  int FindNamedRecord(const std::string& name);


  FieldLayout ParseField(const clang::FieldDecl* decl, uint32_t offset);

 private:
  static std::string GetFileLineColumnStr(const clang::CXXRecordDecl* d);
  static TYPECLASS TypeCheck(const clang::Type* t);

  int HandleCXXRecordKind();

  int HandleField();

 private:
  clang::ASTContext& astctx_;
  std::map<std::string, RecordLayout>& record_layout_;
};

#endif  // MEMLAYOUT_SRC_RECURSIVE_PARSER_H
