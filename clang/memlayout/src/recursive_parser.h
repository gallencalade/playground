#ifndef MEMLAYOUT_SRC_RECURSIVE_PARSER_H
#define MEMLAYOUT_SRC_RECURSIVE_PARSER_H

#include <clang/Frontend/ASTUnit.h>

#include "record_layout.h"

class RecursiveParser {
 public:
  explicit RecursiveParser(clang::ASTContext& ctx,
                           std::map<std::string, RecordLayout>& rl)
      : astctx_(ctx), record_layout_(rl) {  }

  int ParseNamedRecord(const std::string& name);

 private:
  int ParseRecordLayout(const std::string& name,
                        const clang::CXXRecordDecl* decl);

  int ParseEnum(const std::string& name);

  static std::string GetFileLineColumnStr(const clang::CXXRecordDecl* d);

  static TYPECLASS TypeCheck(const clang::Type* t);

  int HandleNestedRecordKind(const std::string& name,
                             const clang::CXXRecordDecl* d);

  FieldLayout HandleField(const clang::FieldDecl* decl, int32_t offset);

 private:
  clang::ASTContext& astctx_;
  std::map<std::string, RecordLayout>& record_layout_;
};

#endif  // MEMLAYOUT_SRC_RECURSIVE_PARSER_H
