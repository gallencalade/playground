#include <iostream>
#include <functional>
#include <map>
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

class DrinkFactory {
 public:
  DrinkFactory() {
    factories["coffe"] = [] {
      return std::make_unique<Coffe>();
    };
    factories["tea"] = [] {
      return std::make_unique<Tea>();
    };
  }

  std::unique_ptr<HotDrink> make_drink(const std::string&& name) {
    return factories[name]();
  }

 private:
  std::map<std::string, std::function<std::unique_ptr<HotDrink>()>> factories;
};

int main(int argc, char* argv[]) {
  DrinkFactory factory;
  auto pt = factory.make_drink("tea");
  auto pc = factory.make_drink("coffe");

  return 0;
}
