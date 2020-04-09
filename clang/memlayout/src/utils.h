#ifndef MEMLAYOUT_SRC_UTILS_H
#define MEMLAYOUT_SRC_UTILS_H

#include <string>

inline std::string remove_prefix(const std::string& s) {
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

#endif  // MEMLAYOUT_SRC_UTILS_H
