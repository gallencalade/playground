#include <cmath>

struct Point {
  friend struct PointFactory;

 private:
  Point(float x, float y)
    : x(x), y(y) {  }

 public:
  float x;
  float y;
};

struct PointFactory {
 public:
  static Point NewCartesian(float x, float y) {
    return { x, y };
  }

  static Point NewPolar(float r, float theta) {
    return { r * std::cos(theta), r * std::sin(theta) };
  }
};

int main() {
  auto cp = PointFactory::NewCartesian(1.0, 2.0);
  auto pp = PointFactory::NewPolar(3.0, 4.0);

  return 0;
}
