#ifndef MEMLAYOUT_SRC_DUMPER_H
#define MEMLAYOUT_SRC_DUMPER_H

#include <map>
#include "mapped_layout.h"

class Dumper {
 public:
  explicit Dumper(const std::map<std::string, RecordLayout>& ml)
      : maplayout_(ml) {  }

  int Dump(const std:string& key, const char* buf, size_t len) const {
    if (maplayout_.end() == maplayout_.find(key)) {
      std::cout << "Cannot find key: " << key << std::endl;
      return -1;
    }
  }

 private:
  int DumpFundamental()

 private:
  const std::map<std::string, RecordLayout>& maplayout_;
};

#endif  // MEMLAYOUT_SRC_DUMPER_H
