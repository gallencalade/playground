#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Tag {
 protected:
  Tag(const std::string& name, const std::string& text)
    : name(name), text(text) {  }

  Tag(const std::string& name, const std::vector<Tag>& children)
    : name(name), children(children) {  }

 public:
  friend std::ostream& operator<<(std::ostream& os, const Tag& tag) {
    // implementation omitted
    return os;
  }

  std::string name;
  std::string text;
  std::vector<Tag> children;
  std::vector<std::pair<std::string, std::string>> attributes;
};

struct P : public Tag {
  explicit P(const std::string& text)
    : Tag("p", text) {  }

  P(std::initializer_list<Tag> children)
    : Tag("p", children) {  }
};

struct IMG : public Tag {
  explicit IMG(const std::string& url)
    : Tag("img", "") {
      attributes.emplace_back(std::make_pair("src", url));
    }
};

int main() {
  std::cout <<
    P {
      IMG { "http://test/test.png" }
    }
  << std::endl;

  return 0;
}
