#include <iostream>
#include <string>

struct Creature {
  std::string name;
  int attack;
  int defense;

  friend std::ostream& operator<<(std::ostream& os, Creature& c) {
    os << c.name << " " << c.attack << " " << c.defense;
    return os;
  }
};

class CreatureModufuer {
  CreatureModufuer* next{nullptr};

 protected:
  Creature& creature;

 public:
  explicit CreatureModufuer(Creature& creature) : creature(creature) {  }

 void add(CreatureModufuer* cm) {
  if (next) { next->add(cm); }
  else { next = cm; }
 }

 virtual void handle() {
  if (next) { next->handle(); }
 }
};

class DoubleAttackModifier : public CreatureModufuer {
 public:
  explicit DoubleAttackModifier(Creature& creature)
    : CreatureModufuer(creature) {  }

  void handle() override {
    creature.attack *= 2;
    CreatureModufuer::handle();
  }
};

class IncreaseDefenseModifier : public CreatureModufuer {
 public:
  explicit IncreaseDefenseModifier(Creature& creature)
    : CreatureModufuer(creature) {  }

  void handle() override {
    if (creature.attack <= 2) { creature.defense += 1; }
    CreatureModufuer::handle();
  }
};

int main() {
  Creature goblin{ "Goblin", 1, 1 };
  CreatureModufuer root{ goblin };
  DoubleAttackModifier r1{ goblin };
  DoubleAttackModifier r1_2{ goblin };
  IncreaseDefenseModifier r2{ goblin };

  root.add(&r1);
  root.add(&r1_2);
  root.add(&r2);

  root.handle();

  std::cout << goblin << std::endl;

  return 0;
}

class NoBonusesModifier : public CreatureModufuer {
 public:
  explicit NoBonusesModifier(Creature& creature)
    : CreatureModufuer(creature) {  }

  void handle() override {
    // nothing here
  }
};
