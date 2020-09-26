#include<iostream>
#include <memory>
#include <vector>

struct Point {
  int x;
  int y;
};

struct Line {
  Point start;
  Point end;
};

struct VectorObject {
  virtual std::vector<Line>::iterator begin() = 0;
  virtual std::vector<Line>::iterator end() = 0;
};

struct VectorRectangle : public VectorObject {
  VectorRectangle(int x, int y, int w, int h) {
    lines_.emplace_back(Line{ Point{ x, y }, Point{ x + w, y } });
    lines_.emplace_back(Line{ Point{ x + w, y }, Point{ x + w, y + h } });
    lines_.emplace_back(Line{ Point{ x, y }, Point{ x, y + h } });
    lines_.emplace_back(Line{ Point{ x, y + h }, Point{ x + w, y + h } });
  }

  std::vector<Line>::iterator begin() override { return lines_.begin(); }

  std::vector<Line>::iterator end() override { return lines_.end(); }

 private:
  std::vector<Line> lines_;
};


struct CPointDC {
  void SetPixel(int x, int y, int w) {
    std::cout << "CPointDC " << x << " " << y << " " << w << std::endl;
  }
};

void DrawPoints(CPointDC& dc,
                std::vector<Point>::iterator start,
                std::vector<Point>::iterator end) {
  for (auto i = start; i != end; ++i) {
    dc.SetPixel(i->x, i->y, 0);
  }
}


struct LineToPointAdapter {
  using Points = std::vector<Point>;

  LineToPointAdapter(Line& line) {
    int left = std::min(line.start.x, line.end.x);
    int right = std::max(line.start.x, line.end.x);
    int top = std::min(line.start.y, line.end.y);
    int bottom = std::max(line.start.y, line.end.y);

    int dx = right - left;
    int dy = line.end.y - line.start.y;

    if (0 == dx) {
      for (int y = top; y <= bottom; ++y) {
        points_.emplace_back(Point{ left, y });
      }
    } else if (0 == dy) {
      for (int x = left; x <= right; ++x) {
        points_.emplace_back(Point{ x, top });
      }
    }
  }

  virtual Points::iterator begin() { return points_.begin(); }
  virtual Points::iterator end() { return points_.end(); }

 private:
  Points points_;
};

int main() {
  CPointDC dc;

  std::vector<std::shared_ptr<VectorObject>> VectorObjects {
    std::make_shared<VectorRectangle>(10, 10, 100, 100),
    std::make_shared<VectorRectangle>(30, 30, 60, 60),
  };

  for (auto& obj : VectorObjects) {
    for (auto& line : *obj) {
      LineToPointAdapter lpo{ line };
      DrawPoints(dc, lpo.begin(), lpo.end());
    }
  }

  return 0;
}
