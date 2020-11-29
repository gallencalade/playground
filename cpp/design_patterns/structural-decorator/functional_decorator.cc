#include <string>
#include <iostream>
#include <functional>

struct Logger {
  Logger(const std::function<void()>& func, const std::string& name)
    : func(func), name(name) {  }

  void operator()() const {
    std::cout << "Entering " << name << std::endl;
    func();
    std::cout << "Exiting " << name << std::endl;
  }

  std::function<void()> func;
  std::string name;
};

template <typename Func>
struct Logger2 {
  Logger2(const Func& func, const std::string& name)
    : func(func), name(name) {  }

  void operator()() const {
    std::cout << "Entering " << name << std::endl;
    func();
    std::cout << "Exiting " << name << std::endl;
  }

  Func func;
  std::string name;
};

template <typename Func>
auto make_logger2(Func func, const std::string& name) {
  return Logger2<Func>(func, name);
}

template <typename R, typename... Args>
struct Logger3 {
  Logger3(std::function<R(Args...)> func, const std::string& name)
    : func(func), name(name) {  }

  R operator()(Args... args) {
    std::cout << "Entering " << name << std::endl;
    R result = func(args...);
    std::cout << "Exiting " << name << std::endl;
    return result;
  }

  std::function<R(Args...)> func;
  std::string name;
};

template <typename R, typename... Args>
auto make_logger3(R (*func)(Args...), const std::string& name) {
  return Logger3<R, Args...>(std::function<R(Args...)>(func), name);
}

inline int add(int a, int b) {
  return a + b;
}

int main() {
  auto logger = make_logger3(add, "Add");
  auto result = logger(2, 3);
  std::cout << result << std::endl;
}
