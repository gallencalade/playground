#include "recursive_parser.h"

int FindNamedDecl(const std::string& name) {
  for (auto a : )
}

int ParseRecordLayout(const std::string& name,
                      const clang::CXXRecordDecl* decl) {
  for (const auto* d : decl->decls()) {
    switch (d->getKind()) {
      case clang::Decl::Kind::CXXRecord : {
        const auto* dd = static_cast<clang::CXXRecordDecl*>(d);
        break;
      }
      case clang::Decl::Kind::Field : {
        const auto* dd = static_cast<clang::FieldDecl*>(d);
        auto fieldlayout = ParseField(dd);
        if (fieldlayout.tycls != TYPECLASS::TC_FUNDAMENTAL) {
          ()
        }
        break;
      }
      default: {
        return -1;
      }
    }
  }
}

FieldLayout ParseField(const clang::FieldDecl* decl) {
  const auto& dtype = f->getType().getDesugaredType(decl->getASTContext());
  std::string type = ::remove_prefix(dtype.getAsString());
  bool builtin = dtype.getTypePtr()->isFundamentalType();
  std::vector<uint32_t> dim({ 1 });
  // return if is array
  if (dtype.getTypePtr()->isConstantArrayType()) {
    const auto* arr = f->getASTContext().getAsConstantArrayType(dtype);
    type = arr->getElementType().getAsString();
    builtin = arr->getElementType().getTypePtr()->isFundamentalType();
    dim[0] = std::stoi(arr->getSize().toString(10, true));

    while (arr->getElementType().getTypePtr()->isConstantArrayType()) {
      arr = f->getASTContext().getAsConstantArrayType(arr->getElementType());
      type = arr->getElementType().getAsString();
      builtin = arr->getElementType().getTypePtr()->isFundamentalType();
      dim.push_back(std::stoi(arr->getSize().toString(10, true)));
    }
}
