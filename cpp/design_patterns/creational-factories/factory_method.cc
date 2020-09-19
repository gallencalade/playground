#include <cmath>

struct Point {
 public:
  static Point NewCartesian(float x, float y) {
    return { x, y };
  }

  static Point NewPolar(float r, float theta) {
    return { r * std::cos(theta), r * std::sin(theta) };
  }

 private:
  Point(const float x, const float y)
    : x(x), y(y) {  }

 private:
  float x;
  float y;
};

int main(int argc, char* argv[]) {
  auto cp = Point::NewCartesian(1.0, 2.0);
  auto pp = Point::NewPolar(3.0, 4.0);

  return 0;
}
