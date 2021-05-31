#ifndef TEST_H
#define TEST_H

#include <map>
#include <vector>

class Offsetter {
 public:
  auto LowerBound(size_t offset, int s) const {
    return s * ((offset + (s - 1)) / s);
  }

  auto FillAt(std::map<size_t, int>::iterator i, int s, int n) {
    auto lb = LowerBound(i->first, s);
    if (i->second > s * n + lb - i->first) {
      m_[lb + s * n] = i->second - s * n - (lb - i->first);

      if (lb > i->first) {
        m_[i->first] = lb - i->first;
      } else {
        m_.erase(i);
      }
    } else if (i->second == s * n + lb - i->first) {
      if (lb > i->first) {
        m_[i->first] = lb - i->first;
      } else {
        m_.erase(i);
      }
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

#endif
