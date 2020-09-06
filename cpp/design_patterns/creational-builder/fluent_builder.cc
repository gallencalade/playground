#include <iostream>
#include <string>
#include <vector>

struct SimpleBuilder;

struct HtmlElement {
  friend struct SimpleBuilder;

 protected:
  // force users to use SimpleBuilder, not HtmlElement
  HtmlElement() = default;
  HtmlElement(const std::string& name, const std::string& text)
    : name(name), text(text) {  }

 public:
  static SimpleBuilder build(const std::string& root_name);

  std::string str(int indent = 0) const {
    return name + " : " + text;
  }

  std::string name;
  std::string text;
  std::vector<HtmlElement> elements;
};

struct SimpleBuilder {
  SimpleBuilder(const std::string& root_name) { root.name = root_name; }

  operator HtmlElement() const { return root; }

  // returns pointer to SimpleBuilder to fluent call
  SimpleBuilder add_child(const std::string& child_name, const std::string& child_test) {
    HtmlElement e(child_name, child_test);
    root.elements.emplace_back(e);

    return *this;
  }

  std::string str() const { return root.str(); }

  HtmlElement root;
};

/* static */
SimpleBuilder HtmlElement::build(const std::string& root_name) {
  return SimpleBuilder(root_name);
}

int main() {
  auto builder = HtmlElement::build("ul");
  builder.add_child("li", "hello").add_child("li", "world");

  return 0;
}
