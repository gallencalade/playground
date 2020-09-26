#include <iostream>

struct Renderer {
  virtual void render_circle(float x, float y, float radius) = 0;
};

struct VectorRenderer : public Renderer {
  void render_circle(float x, float y, float radius) override {
    std::cout << "Rasterizing circle of radius " << radius << std::endl;
  }
};

struct RasterRerenderer : Renderer {
  void render_circle(float x, float y, float radius) override {
    std::cout << "Drawing a vector circle of radius " << radius << std::endl;
  }
};


struct Shape {
 public:
  virtual void draw() = 0;
  virtual void resize(float factor) = 0;

 protected:
  Shape(Renderer& renderer) : renderer(renderer) {  }
  Renderer& renderer;
};

struct Circle : public Shape {
  Circle(Renderer& renderer, float x, float y, float radius)
    : Shape(renderer), x(x), y(y), radius(radius) {  }

  void draw() override { renderer.render_circle(x, y, radius); }

  void resize(float factor) override { radius *= factor; }

  float x;
  float y;
  float radius;
};

int main() {
  RasterRerenderer rr;
  Circle raster_circle(rr, 5, 5, 5);
  raster_circle.draw();
  raster_circle.resize(2);
  raster_circle.draw();

  return 0;
}
