#include <iostream>
#include <variant>

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Beginning {};
struct Develop {};
struct Climax {};
struct End {};

int main() {
  std::variant<Beginning, Develop, Climax, End> state;

  for (int i = 0; i < 5; ++i) {
    std::visit(
          overloaded{
              [&state](const Beginning& st) {
                std::cout << "Beginning" << std::endl;
                state = Develop();
              }, [&state](const Develop& st) {
                std::cout << "Develop" << std::endl;
                state = Climax();
              }, [&state](const Climax& st) {
                std::cout << "Climax" << std::endl;
                state = End();
              }, [&state](const End& st) {
                std::cout << "End" << std::endl;
                state = Beginning();
              }
            },
          state);
  }

  return 0;
}
