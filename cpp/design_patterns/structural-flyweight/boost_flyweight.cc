#include <string>
#include <iostream>

#include <boost/flyweight.hpp>

struct User2 {
  boost::flyweight<std::string> first_name, last_name;

  User2(const std::string& first_name,
        const std::string& last_name)
      : first_name{ first_name }
      , last_name{ last_name } { }
};

int main() {
  User2 john_doe{ "John", "Doe" };
  User2 jane_doe{ "Jane", "Doe" };

  std::cout << (&jane_doe.last_name.get() == &john_doe.last_name.get())
            << std::endl;
}
