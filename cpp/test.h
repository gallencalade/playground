#ifndef TEST_H
#define TEST_H

#include <map>
#include <iostream>
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
      last_ = (last_ < lb + s * n ? lb + s * n : last_);

      if (lb > i->first) {
        m_[i->first] = lb - i->first;
      } else {
        m_.erase(i);
      }
    } else if (i->second == s * n + lb - i->first) {
      last_ = (last_ < i->first + i->second ? i->first + i->second : last_);
      if (lb > i->first) {
        m_[i->first] = lb - i->first;
      } else {
        m_.erase(i);
      }
    } else {
      int f = s * n - (i->second - (lb - i->first));
      last_ = i->first + i->second;
      size_t r = FillNew(s, f / s);
      if (lb > i->first) {
        m_[i->first] = lb - i->first;
      } else {
        m_.erase(i);
      }
    }

    return lb;
  }

  size_t FillNew(int s, int n = 1) {
    std::size_t r = last_;
    last_ += s * n;
    if (0 != LowerBound(last_, 8) - last_) {
      m_[last_] = LowerBound(last_, 8) - last_;
    }
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
