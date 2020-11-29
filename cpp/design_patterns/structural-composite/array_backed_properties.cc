#include <array>
#include <numeric>
#include <algorithm>
#include <iostream>

class Creature {
 public:
  Creature() {
    abilities[str] = 10;
    abilities[agl] = 20;
    abilities[intl] = 30;
  }

  int get_strength() const { return abilities[str]; }

  void set_strength(int value) { abilities[str] = value; }

  int sum() const {
    return std::accumulate(abilities.begin(), abilities.end(), 0);
  }

  double average() const {
    return sum() / (double)count;
  }

  int max() const {
    return *std::max_element(abilities.begin(), abilities.end());
  }

 private:
  enum Abilities { str, agl, intl, count };
  std::array<int, count> abilities;
};

int main() {
  Creature creature;
  std::cout << creature.get_strength() << " "
            << creature.sum() << " "
            << creature.average() << " "
            << creature.max() << std::endl;
}
