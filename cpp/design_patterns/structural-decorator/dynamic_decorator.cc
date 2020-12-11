#include <string>
#include <sstream>
#include <iostream>

struct Shape {
  virtual std::string str() const { return ""; };
};

struct Circle : public Shape {
  explicit Circle(const float radius) : radius(radius) {  }

  void resize(float factor) { radius *= factor; }

  std::string str() const override {
    std::ostringstream oss;
    oss << "A circle of radius " << radius;
    return oss.str();
  }

  float radius;
};

struct ColoredShape : public Shape {
  ColoredShape(Shape& shape, const std::string& color)
    : shape(shape), color(color) {  }

  std::string str() const override {
    std::ostringstream oss;
    oss << shape.str() << " has the color " << color;
    return oss.str();
  }

  Shape& shape;
  std::string color;
};

struct TransparentShape : Shape {
  TransparentShape(Shape& shape, uint8_t transparency)
    : shape(shape), transparency(transparency) {  }

  std::string str() const override {
    std::ostringstream oss;
    oss << shape.str() << " has " << static_cast<float>(transparency) / 255.f * 100.f
        << "% transparency";
    return oss.str();
  }

  Shape& shape;
  uint8_t transparency;
};

int main() {
//   TransparentShape myCircle {
//     ColoredShape {
//       Circle{23}, "green"
//     }, 64
//   };
  Circle circle(23);
  ColoredShape coloredShape(circle, "green");
  TransparentShape transparentShape(coloredShape, 64);

  std::cout << transparentShape.str() << std::endl;

  return 0;
}
