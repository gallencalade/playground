#include <iostream>
#include <string>
#include <vector>

struct HtmlElement {
  HtmlElement() = default;
  HtmlElement(const std::string& name, const std::string& text)
    : name(name), text(text) {  }

  std::string str(int indent = 0) const {
    return name + " : " + text;
  }

  std::string name;
  std::string text;
  std::vector<HtmlElement> elements;
};

struct SimpleBuilder {
  SimpleBuilder(const std::string& root_name) { root.name = root_name; }

  void add_child(const std::string& child_name, const std::string& child_test) {
    HtmlElement e(child_name, child_test);
    root.elements.emplace_back(e);
  }

  std::string str() const { return root.str(); }

  HtmlElement root;
};
