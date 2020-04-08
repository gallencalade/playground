#ifndef MEMLAYOUT_SRC_MEMLAYOUT_H
#define MEMLAYOUT_SRC_MEMLAYOUT_H

#include <memory>
#include <vector>

#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/Tooling.h>

#include "func_temp_arg_type_finder.h"
#include "namespace_struct_finder.h"

enum TYPECLASS {
  TC_UNKNOWN,
  TC_UNION,
  TC_STRUCT,
  TC_FUNDAMENTAL,
  TC_ENUM,
};

struct FieldLayout {
  std::string type;   // type name: int, char, classA, structB, ...
  std::string var;
  TYPECLASS tycls;
  uint32_t offset;
  std::vector<uint32_t> dim;
};

struct RecordLayout {
  std::string name;   // classA, structB, unionC, ...
  TYPECLASS tycls;
  std::vector<FieldLayout> fields;
};

inline std::ostream& operator<<(std::ostream& os, const FieldLayout& f) {
  os << "+ type  : " << f.type << std::endl;
  os << "  var   : " << f.var << std::endl;
  os << "  tycls : " << f.tycls << std::endl;
  os << "  offset: " << f.offset << std::endl;
  os << "  dim   : ";
  for (const auto a : f.dim) {
    os << a << " ";
  }

  return os;
}

inline std::ostream& operator<<(std::ostream& os, const RecordLayout& r) {
  os << "---------" << std::endl;
  os << "name   : " << r.name << std::endl;
  os << "tycls  : " << r.tycls << std::endl;
  os << "fields : " << std::endl;
  for (const auto& a : r.fields) {
    os << a << std::endl;
  }

  return os;
}

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
