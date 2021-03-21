template <typename T>
struct Property {
  T value;

  Property(const T init_val) { *this = init_val; }

  operator T() { return value; }

  T operator=(T new_val) { return value = new_val; }
};

struct Creature {
  Property<int> strength{ 10 };
  Property<int> agility{ 5 };
};

int main() {
  Creature creature;
  creature.agility = 20;
  auto x = creature.strength;
}
