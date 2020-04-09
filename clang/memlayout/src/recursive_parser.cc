#include <clang/AST/RecordLayout.h>

#include "dynamic_finder.h"
#include "recursive_parser.h"

#include <iostream>

namespace {

std::string remove_type_qualifier(const std::string& s) {
  std::string ss(s);

  if (std::string::npos != ss.find("struct ")) {
    ss.erase(0, sizeof("struct ") - 1);
  }
  if (std::string::npos != ss.find("union ")) {
    ss.erase(0, sizeof("union ") - 1);
  }
  if (std::string::npos != ss.find("enum ")) {
    ss.erase(0, sizeof("enum ") - 1);
  }

  return ss;
}

} // namespace

int RecursiveParser::Parse(const std::string& name) {
  auto rname = remove_type_qualifier(name);
  auto bns = DynamicFinder::Find<DynamicFinder::RecordFinderStr>(astctx_, rname);
  if (bns.empty()) {
    return 1;   // not found in this astctx_;
  }
  if (bns.size() > 1) {
    llvm::errs() << "There might be muti definitions of " << name << "\n";
    return -1;
  }

  const auto* decl = bns.at(0).getNodeAs<clang::CXXRecordDecl>("matched");

  return ParseRecordLayout(rname, decl);
}

int RecursiveParser::ParseRecordLayout(const std::string& name,
                                       const clang::CXXRecordDecl* decl) {
  if (record_layout_.find(name) != record_layout_.end()) {
    return 0;
  }

  const auto& layout = decl->getASTContext().getASTRecordLayout(decl);
  std::vector<FieldLayout> fields_layout;
  fields_layout.reserve(layout.getFieldCount());

  for (const auto* d : decl->decls()) {
    if (clang::Decl::Kind::CXXRecord == d->getKind()) {
      const auto* dd = static_cast<const clang::CXXRecordDecl*>(d);
      if (dd->hasDefinition()) {
        const auto& tstr = dd->getNameAsString();
        if (tstr.empty()) {    // no type name
          std::string apps(name, 0, name.find("::(anonymous at"));
          apps.append(GetFileLineColumnStr(dd));
          ParseRecordLayout(apps, dd);
        } else {
          ParseRecordLayout(name + "::" + tstr, dd);
        }
      }
    }
  }

  for (const auto* d : decl->fields()) {
    if (d->getType().getTypePtr()->isEnumeralType()) {
      auto bns = DynamicFinder::Find<DynamicFinder::EnumFinderStr>(decl->getASTContext(), name);
    } else {
      std::string tstr = ::remove_type_qualifier(d->getType().getDesugaredType(decl->getASTContext()).getAsString());
      auto offset = layout.getFieldOffset(d->getFieldIndex()) / 8;
      auto field = ParseField(d, offset);
      if (field.tycls != TYPECLASS::TC_FUNDAMENTAL) {
        if (record_layout_.find(tstr) == record_layout_.end()) {
          Parse(tstr);
        }
      }
      fields_layout.push_back(field);
    }
  }

  RecordLayout record;
  record.name = name;
  record.tycls = TypeCheck(decl->getTypeForDecl());
  record.fields = std::move(fields_layout);

  record_layout_[record.name] = record;

  return 0;
}

FieldLayout RecursiveParser::ParseField(const clang::FieldDecl* decl,
                                        uint32_t offset) {
  const auto& dtype = decl->getType().getDesugaredType(decl->getASTContext());
  const auto* ptype = dtype.getTypePtr();

  std::string type = ::remove_type_qualifier(dtype.getAsString());
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

std::string RecursiveParser::GetFileLineColumnStr(
      const clang::CXXRecordDecl* d) {
  const auto& ctx = d->getASTContext();
  const auto& loc = d->getBeginLoc();

  std::string f(ctx.getSourceManager().getFilename(loc).str());
  auto l = ctx.getSourceManager().getSpellingLineNumber(loc);
  auto c = ctx.getSourceManager().getSpellingColumnNumber(loc);

  return std::string("::(anonymous at " + f + ":" + std::to_string(l) + ":" +
        std::to_string(c) + ")");
}

TYPECLASS RecursiveParser::TypeCheck(const clang::Type* t) {
  if (t->isFundamentalType()) { return TYPECLASS::TC_FUNDAMENTAL; }
  if (t->isUnionType()) { return TYPECLASS::TC_UNION; }
  if (t->isStructureOrClassType()) { return TYPECLASS::TC_STRUCT; }
  if (t->isEnumeralType()) { return TYPECLASS::TC_ENUM; }

  return TYPECLASS::TC_UNKNOWN;
}
