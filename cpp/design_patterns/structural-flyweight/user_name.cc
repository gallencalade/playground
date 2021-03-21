#include <cstdint>
#include <iostream>
#include <string>

#include <boost/bimap/bimap.hpp>
#include <boost/bimap.hpp>

using key = uint32_t;

struct User {
  User(const std::string& first_name,
       const std::string& last_name)
      : first_name{ add(first_name) },
    last_name{ add(last_name) } { }

//   const std::string& get_first_name() const {
//     return names.left.find(last_name)->second;
//   }
// 
//   const std::string& get_last_name() const {
//     return names.left.find(last_name)->second;
//   }

  friend std::ostream& operator<<(std::ostream& os, const User& obj) {
//     return os << obj.get_first_name() << " " << obj.get_last_name();
  }

 protected:
  key first_name;
  key last_name;
  static boost::bimap<key, std::string> names;
  static key seed;
  static key add(const std::string& s) {
    std::cout << typeid(names.right).name() << std::endl;
//     auto it = names.right.find(s);
//     if (it == names.right.end()) {
//       names.insert({ ++seed, s });
//       return seed;
//     }

    return 0; // it->second;
  }
};

int main() {
  User u1("A", "B");
  User u2("A", "C");

  std::cout << u1 << std::endl << u2 << std::endl;

  return 0;
}
