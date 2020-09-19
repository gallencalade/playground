#include <iostream>
#include <memory>

struct HotDrink {
  virtual void prepare(int volume) = 0;
};

struct Tea : HotDrink {
  void prepare(int volume) override {
    std::cout << "Tea: " << volume << std::endl;
  }
};

struct Coffe : HotDrink {
  void prepare(int volume) override {
    std::cout << "Coffe: " << volume << std::endl;
  }
};

std::unique_ptr<HotDrink> make_drink(std::string&& type) {
  if (type == "tea") {
    auto p = std::make_unique<Tea>();
    p->prepare(200);
    return p;
  } else {
    auto p = std::make_unique<Coffe>();
    p->prepare(300);
    return p;
  }
}

int main(int argc, char* argv[]) {
  auto pt = make_drink("tea");
  auto pc = make_drink("coffe");

  return 0;
}
