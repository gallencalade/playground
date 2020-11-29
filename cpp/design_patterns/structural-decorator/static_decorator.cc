#include <string>
#include <sstream>
#include <iostream>

struct Shape {
  virtual std::string str() const { return ""; };
};

struct Circle : public Shape {
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

  std::string str() const override {
    std::ostringstream oss;
    oss << T::str() << " has the transparency " << transparency;
    return oss.str();
  }

  uint8_t transparency;
};

int main() {
  ColoredShape<TransparentShape<Circle>> circle;
  circle.radius = 20.0;
  circle.color = "green";
  circle.transparency = 32;

  std::cout << circle.str() << std::endl;

  return 0;
}
