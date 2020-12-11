#include<iostream>
#include <memory>
#include <vector>
#include <map>

#include <boost/container_hash/hash.hpp>

struct Point {
  int x;
  int y;

  friend std::size_t hash_value(const Point& obj) {
    std::size_t seed = 0x725C686F;
    boost::hash_combine(seed, obj.x);
    boost::hash_combine(seed, obj.y);
    return seed;
  }
};

struct Line {
  Point start;
  Point end;

  friend std::size_t hash_value(const Line& obj) {
    std::size_t seed = 0x719E6B16;
    boost::hash_combine(seed, obj.start);
    boost::hash_combine(seed, obj.end);
    return seed;
  }
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


using Points = std::vector<Point>;

struct LineToPointCachingAdapter {
  LineToPointCachingAdapter(Line& line) {
    static boost::hash<Line> hash;
    line_hash_ = hash(line);
    if (cache_.contains(line_hash_)) { return; }

    int left = std::min(line.start.x, line.end.x);
    int right = std::max(line.start.x, line.end.x);
    int top = std::min(line.start.y, line.end.y);
    int bottom = std::max(line.start.y, line.end.y);

    int dx = right - left;
    int dy = line.end.y - line.start.y;

    Points points;
    if (0 == dx) {
      for (int y = top; y <= bottom; ++y) {
        points.emplace_back(Point{ left, y });
      }
    } else if (0 == dy) {
      for (int x = left; x <= right; ++x) {
        points.emplace_back(Point{ x, top });
      }
    }

    cache_[line_hash_] = points;
  }

  virtual Points::iterator begin() { return cache_[line_hash_].begin(); }
  virtual Points::iterator end() { return cache_[line_hash_].end(); }

 private:
  static std::size_t line_hash_;
  static std::map<size_t, Points> cache_;
};

std::size_t LineToPointCachingAdapter::line_hash_;
std::map<size_t, Points> LineToPointCachingAdapter::cache_;

int main() {
  CPointDC dc;

  std::vector<std::shared_ptr<VectorObject>> VectorObjects {
    std::make_shared<VectorRectangle>(10, 10, 100, 100),
    std::make_shared<VectorRectangle>(30, 30, 60, 60),
  };

  for (auto& obj : VectorObjects) {
    for (auto& line : *obj) {
      LineToPointCachingAdapter lpo{ line };
      DrawPoints(dc, lpo.begin(), lpo.end());
    }
  }

  return 0;
}
