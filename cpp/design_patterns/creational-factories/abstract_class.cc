#include <iostream>
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

struct HotDrinkFactory {
  virtual std::unique_ptr<HotDrink> make() const = 0;
};

struct CoffeFactory : HotDrinkFactory {
  std::unique_ptr<HotDrink> make() const override {
    auto p = std::make_unique<Coffe>();
    return p;
  }
};

struct TeaFactory : HotDrinkFactory {
  std::unique_ptr<HotDrink> make() const override {
    auto p = std::make_unique<Tea>();
    return p;
  }
};

class DrinkFactory {
 public:
  DrinkFactory() {
    hot_factories["coffe"] = std::make_unique<CoffeFactory>();
    hot_factories["tea"] = std::make_unique<TeaFactory>();
  }

  std::unique_ptr<HotDrink> make_drink(const std::string&& name) {
    return hot_factories[name]->make();
  }

 private:
  std::map<std::string, std::unique_ptr<HotDrinkFactory>> hot_factories;
};

int main(int argc, char* argv[]) {
  DrinkFactory factory;
  auto pt = factory.make_drink("tea");
  auto pc = factory.make_drink("coffe");

  return 0;
}
