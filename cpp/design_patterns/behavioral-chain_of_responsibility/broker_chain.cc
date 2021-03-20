#include <iostream>

#include <boost/signals2.hpp>

struct Query {
  std::string name;
  enum Arg { attack, defense } argument;
  int result;
};

struct Game {
  boost::signals2::signal<void(Query&)> queries;
};

class Creature {
  Game& game;

 public:
  std::string name;

  Creature(Game& game, const std::string& name, int attack, int defense)
      : game(game), name(name), attack(attack), defense(defense) {  }

  int get_attack() const {
    Query q{ name, Query::Arg::attack, attack };
    game.queries(q);
    return q.result;
  }

  int get_defense() const {
    Query q{ name, Query::Arg::defense, defense };
    game.queries(q);
    return q.result;
  }

 private:
  int attack, defense;

  friend std::ostream& operator<<(std::ostream& os, const Creature& c) {
    os << c.name << " " << c.get_attack() << " " << c.defense;
    return os;
  }
};

class CreatureModifier {
  Game& game;
  Creature& creature;

 public:
  CreatureModifier(Game& game, Creature& creature)
      : game(game), creature(creature) {  }
};

class DoubleAttackModifier : public CreatureModifier {
  boost::signals2::connection conn;

 public:
  DoubleAttackModifier(Game& game, Creature& creature)
      : CreatureModifier(game, creature) {
    conn = game.queries.connect([&](Query& q) {
        if (q.name == creature.name &&
            q.argument == Query::Arg::attack) {
          q.result *= 2;
        }
    });
  }

  ~DoubleAttackModifier() { conn.disconnect(); }
};

int main() {
  Game game;
  Creature goblin{ game, "Strong Goblin", 2, 2 };
  std::cout << goblin << std::endl;

  {
    DoubleAttackModifier dam{ game, goblin };
    std::cout << goblin << std::endl;
  }

  std::cout << goblin << std::endl;

  return 0;
}
