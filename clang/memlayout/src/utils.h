#ifndef MEMLAYOUT_SRC_UTILS_H
#define MEMLAYOUT_SRC_UTILS_H

#include <vector>
#include <string>

#include <iostream>

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

inline std::vector<std::string> split_namespace_struct(std::string& s) {
  std::vector<std::string> tokens;
  size_t pos;
  static const std::string d = "::";
  while ((pos = s.find(d)) != std::string::npos) {
    tokens.emplace_back(s.substr(0, pos));
    s.erase(0, pos + d.length());
  }

  return tokens;
}


#endif  // MEMLAYOUT_SRC_UTILS_H
