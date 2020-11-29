#include <string>
#include <sstream>
#include <iostream>

struct Shape {
  virtual std::string str() const { return ""; };
};

struct Circle : public Shape {
  explicit Circle(float radius) : radius(radius) {  }

  void resize(float factor) { radius *= factor; }

  std::string str() const override {
    std::ostringstream oss;
    oss << "A circle of radius " << radius;
    return oss.str();
  }

  float radius;
};

template <typename T>
struct ColoredShape : public T {
  static_assert(std::is_base_of<Shape, T>::value,
                "Template argument must be a Shape");

  template<typename... Args>
  ColoredShape(const std::string& color, Args... args)
      : T(std::forward<Args>(args)...), color(color) {  }

  std::string str() const override {
    std::ostringstream oss;
    oss << T::str() << " has the color " << color;
    return oss.str();
  }

  std::string color;
};

template <typename T>
struct TransparentShape : public T {
  static_assert(std::is_base_of<Shape, T>::value,
                "Template argument must be a Shape");

  template<typename... Args>
  TransparentShape(const uint8_t transparency, Args... args)
      : T(std::forward<Args>(args)...), transparency(transparency) {  }

  std::string str() const override {
    std::ostringstream oss;
    oss << T::str() << " has the transparency " << transparency;
    return oss.str();
  }

  uint8_t transparency;
};

int main() {
  ColoredShape<TransparentShape<Circle>> circle( "red", 51, 5);

  std::cout << circle.str() << std::endl;

  return 0;
}
