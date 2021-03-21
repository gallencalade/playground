#include <iostream>
#include <string>

struct Image {
  virtual void draw() = 0;

  ~Image() {  }
};

struct Bitmap : Image {
  Bitmap(const std::string& filename) : filename{ filename } {
    std::cout << "Loading image from " << filename << std::endl;
  }

  void draw() override {
    std::cout << "Drawing image " << filename << std::endl;
  }

 private:
  std::string filename;
};

struct LazyBitmap : Image {
  LazyBitmap(const std::string& filename) : filename{ filename } {  }

  ~LazyBitmap() { delete bmp; }

  void draw() override {
    if (!bmp) { bmp = new Bitmap(filename); }
    bmp->draw();
  }

 private:
  Bitmap *bmp{ nullptr };
  std::string filename;
};

void draw_image(Image& img) {
  img.draw();
}

int main() {
  LazyBitmap img("pokemon.png");
  draw_image(img);
}
