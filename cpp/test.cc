#include <gtest/gtest.h>

#include "test.h"

TEST(Main, Test) {
  {
    Offsetter setter;
    auto r = setter.FillNew(sizeof(char));
    auto f = setter.Find(sizeof(int), 1);
    EXPECT_EQ(4, setter.FillAt(f, sizeof(int), 1));
    EXPECT_EQ(setter.m_.size(), 1);
    EXPECT_EQ(setter.m_[1], 3);
  }
  {
    Offsetter setter;
    auto r = setter.FillNew(sizeof(char));
    auto f = setter.Find(sizeof(short), 3);
    EXPECT_EQ(2, setter.FillAt(f, sizeof(short), 3));
    EXPECT_EQ(setter.m_.size(), 1);
    EXPECT_EQ(setter.m_[1], 1);
  }
  {
    Offsetter setter;
    auto r = setter.FillNew(sizeof(char));
    auto f = setter.Find(sizeof(char), 7);
    EXPECT_EQ(1, setter.FillAt(f, sizeof(char), 7));
    EXPECT_EQ(setter.m_.size(), 0);
  }

  {
    Offsetter setter;
    auto r = setter.FillNew(sizeof(char));
    auto f = setter.Find(sizeof(short), 2);
    EXPECT_EQ(f->first, setter.last_);
    EXPECT_EQ(2, setter.FillAt(f, sizeof(short), 2));
    EXPECT_EQ(setter.m_.size(), 2);
    EXPECT_EQ(setter.m_[1], 1);
    EXPECT_EQ(setter.m_[6], 2);
  }
  {
    Offsetter setter;
    auto r = setter.FillNew(sizeof(char));
    auto f = setter.Find(sizeof(char), 6);
    EXPECT_EQ(f->first, setter.last_);
    EXPECT_EQ(1, setter.FillAt(f, sizeof(char), 6));
    EXPECT_EQ(setter.m_.size(), 1);
    EXPECT_EQ(setter.m_[7], 1);
  }

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
