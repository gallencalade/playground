#include <map>
#include <vector>
#include <gtest/gtest.h>

class Offsetter {
 public:
  auto LowerBound(size_t offset, int s) const {
    return s * (offset + (s - 1)) / s;
  }

  auto FillAt(std::map<size_t, int>::iterator i, int s, int n) {
    auto lb = LowerBound(i->first, s);
    if (i->second > s * n) {
      m_[lb + s * n] = i->second - s * n;
    }
    if (lb > i->first) {
      m_[i->first] = lb - i->first;
    }

    return lb;
  }

  auto FillNew(int s, int n = 1) {
    auto r = last_;
    last_ += s * n;
    m_[last_] = LowerBound(last_, 8) - last_;
    return r;
  }

  auto Find(int s, int n = 1) {
    for (auto i = m_.begin(); i != m_.end(); ++i) {
      if (i->second >= s * n) {
        return i;
      }
    }

    return m_.find(last_);
  }

  std::size_t last_ = 0;
  std::map<size_t, int> m_;
};

std::vector<size_t> reorder(const std::vector<std::pair<int, int>>& ms) {
  std::vector<size_t> r;
  std::map<size_t, int> m;
  for (auto [s, n] : ms) {
  }

  return r;
}

TEST(Main, Test) {
  std::vector<std::pair<int, int>> ms {
    { sizeof(char), 1 }
  };

  {
    Offsetter setter;
    auto f = setter.Find(sizeof(char));
    EXPECT_EQ(f, setter.m_.end());
    auto r = setter.FillNew(sizeof(char));
    EXPECT_EQ(0, r);
    EXPECT_EQ(setter.m_[setter.last_], 7);
  }
  {
    Offsetter setter;
    auto f = setter.Find(sizeof(char), 6);
    EXPECT_EQ(f, setter.m_.end());
    auto r = setter.FillNew(sizeof(char), 6);
    EXPECT_EQ(0, r);
    EXPECT_EQ(setter.m_[setter.last_], 2);
  }
  {
    Offsetter setter;
    auto f = setter.Find(sizeof(char), 9);
    EXPECT_EQ(f, setter.m_.end());
    auto r = setter.FillNew(sizeof(char), 9);
    EXPECT_EQ(0, r);
    EXPECT_EQ(setter.m_[setter.last_], 7);
  }
}
