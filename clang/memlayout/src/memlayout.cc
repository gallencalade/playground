#include <clang/AST/RecordLayout.h>

#include "memlayout.h"
#include "utils.h"

#include <iostream>

MemLayout::MemLayout(const clang::tooling::CompilationDatabase& compdb,
                     const std::vector<std::string>& files)
    : tool_(compdb, files) {
  if (tool_.buildASTs(asts_)) {
    throw std::runtime_error("Failed to build ASTs");
  }
}

int MemLayout::FindNamedRecord(const std::string& name) {
  std::vector<clang::ast_matchers::BoundNodes> bns;
  for (auto& a : asts_) {
    bns = NamespaceStructFinder::Run(a->getASTContext(), name);
    if (!bns.empty()) {
      break;
    }
  }
  ParseRecordLayout(name, bns.at(0).getNodeAs<clang::CXXRecordDecl>("matched"));

  return 0;
}

static std::string GetFileLineColumn(const clang::CXXRecordDecl* d) {
  const auto& ctx = d->getASTContext();
  const auto& loc = d->getBeginLoc();

  std::string f(ctx.getSourceManager().getFilename(loc).str());
  auto l = ctx.getSourceManager().getSpellingLineNumber(loc);
  auto c = ctx.getSourceManager().getSpellingColumnNumber(loc);

  return std::string("::(anonymous at " + f + ":" + std::to_string(l) + ":" +
        std::to_string(c) + ")");
}


int MemLayout::ParseRecordLayout(const std::string& name,
                      const clang::CXXRecordDecl* decl) {
  if (record_layout_.find(name) != record_layout_.end()) {
    return 0;
  }

  std::cout << name << std::endl;

  const auto& layout = decl->getASTContext().getASTRecordLayout(decl);
  std::vector<FieldLayout> fields_layout;
  fields_layout.reserve(layout.getFieldCount());

  for (const auto* d : decl->decls()) {
    if (clang::Decl::Kind::CXXRecord == d->getKind()) {
      const auto* dd = static_cast<const clang::CXXRecordDecl*>(d);
      if (dd->hasDefinition()) {
        const auto& tstr = dd->getNameAsString();
        if (tstr.empty()) {    // no type name
          std::cout << "000" << std::endl;
          std::string apps(name, 0, name.find("::(anonymous at"));
          apps.append(GetFileLineColumn(dd));
          ParseRecordLayout(apps, dd);
        } else {
          ParseRecordLayout(name + "::" + tstr, dd);
        }
      }
    }
  }

  for (const auto* d : decl->fields()) {
    if (d->getType().getTypePtr()->isEnumeralType()) {
      std::cout << "ENUM" << std::endl;
      break;
    }
    std::string tstr = ::remove_prefix(d->getType().getDesugaredType(decl->getASTContext()).getAsString());
    auto offset = layout.getFieldOffset(d->getFieldIndex()) / 8;
    auto field = ParseField(d, offset);
    if (field.tycls != TYPECLASS::TC_FUNDAMENTAL) {
      if (record_layout_.find(tstr) == record_layout_.end()) {
        FindNamedRecord(tstr);
      }
    }
    fields_layout.push_back(field);
  }

  RecordLayout record;
  record.name = name;
  record.tycls = TypeCheck(decl->getTypeForDecl());
  record.fields = std::move(fields_layout);

  record_layout_[record.name] = record;

  return 0;
}

TYPECLASS MemLayout::TypeCheck(const clang::Type* t) {
  if (t->isFundamentalType()) { return TYPECLASS::TC_FUNDAMENTAL; }
  if (t->isUnionType()) { return TYPECLASS::TC_UNION; }
  if (t->isStructureOrClassType()) { return TYPECLASS::TC_STRUCT; }
  if (t->isEnumeralType()) { return TYPECLASS::TC_ENUM; }

  return TYPECLASS::TC_UNKNOWN;
}

FieldLayout MemLayout::ParseField(const clang::FieldDecl* decl, uint32_t offset) {
  const auto& dtype = decl->getType().getDesugaredType(decl->getASTContext());
  const auto* ptype = dtype.getTypePtr();

  std::string type = ::remove_prefix(dtype.getAsString());
  TYPECLASS tycls = TypeCheck(ptype);
  std::vector<uint32_t> dim({ 1 });
  // return if is array
  if (ptype->isConstantArrayType()) {
    const auto* arr = decl->getASTContext().getAsConstantArrayType(dtype);
    type = arr->getElementType().getAsString();
    tycls = TypeCheck(arr->getElementType().getTypePtr());
    dim[0] = std::stoi(arr->getSize().toString(10, true));

    while (arr->getElementType().getTypePtr()->isConstantArrayType()) {
      arr = decl->getASTContext().getAsConstantArrayType(arr->getElementType());
      type = arr->getElementType().getAsString();
      tycls = TypeCheck(arr->getElementType().getTypePtr());
      dim.push_back(std::stoi(arr->getSize().toString(10, true)));
    }
  }

  return FieldLayout({ type, decl->getNameAsString(), tycls, offset,
                       std::move(dim) });
}




std::map<std::string, std::vector<StructField>> MemLayout::Run() {
  std::map<std::string, std::vector<StructField>> layouts;
  auto struct_names = FuncTempArgTypeQuery();
  for (const auto& a : struct_names) {
    FindNamedRecord(::remove_prefix(a));
    /*if (0 != RecursiveParse(remove_prefix(a), layouts)) {
      return std::map<std::string, std::vector<StructField>>();
    }*/
  }

  for (auto [k, v] : record_layout_) {
    std::cout << "key: " << k << std::endl << "value: " << v << "\n";
  }

  return layouts;
}

/*int MemLayout::RecursiveParse(const std::string& st,
      std::map<std::string, std::vector<StructField>>& mapper) {
  if (mapper.end() != mapper.find(st)) {
    return 0;
  }

  auto l = NamespaceStructLayoutQuery(st);
  if (l.empty()) {
    return -1;
  }

  mapper[st] = l;

  for (const auto& a : l) {
    if (!a.builtin && (mapper.end() == mapper.find(a.type))) {
      if (0 != RecursiveParse(a.type, mapper)) {
        return -1;
      }
    }
  }

  return 0;
}

std::vector<StructField> MemLayout::NamespaceStructLayoutQuery(
      const std::string& st) {
  std::vector<StructField> result;
  for (auto& a : asts_) {
    result = NamespaceStructFinder::Run(a->getASTContext(), st);
    if (!result.empty()) {
      return result;
    }
  }

  llvm::errs() << "No definition found: " << st << "\n";

  return result;
}*/

std::set<std::string> MemLayout::FuncTempArgTypeQuery() {
  std::set<std::string> result;
  for (auto& a : asts_) {
    auto r = FuncTempArgTypeFinder::Run(a->getASTContext());
    result.merge(r);
  }

  return result;
}
