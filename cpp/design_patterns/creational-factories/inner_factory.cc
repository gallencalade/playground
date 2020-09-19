#include <cmath>

struct Point {
 private:
  Point(float x, float y)
    : x(x), y(y) {  }

 public:
  struct PointFactory {
    static Point NewCartesian(float x, float y) {
      return { x, y };
    }

    static Point NewPolar(float r, float theta) {
      return { r * std::cos(theta), r * std::sin(theta) };
    }
  };

  static PointFactory Factory;

 private:
  float x;
  float y;
};

int main(int argc, char* argv[]) {
  auto cp = Point::PointFactory::NewCartesian(1.0, 2.0);
  auto pp = Point::PointFactory::NewPolar(3.0, 4.0);

  return 0;
}
