#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../queue.h"

namespace ds {

TEST(QueueTest, Constuctor) {
  Queue<int> a;
  EXPECT_EQ(a.Size(), 0);
}

TEST(QueueTest, PushPeekPop) {
  Queue<int> a;
  EXPECT_EQ(a.Size(), 0);

  a.Push(7);
  EXPECT_EQ(a.Size(), 1);
  EXPECT_EQ(a.Peek(), 7);

  a.Push(6);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(a.Peek(), 7);

  EXPECT_EQ(a.Pop(), 7);
  EXPECT_EQ(a.Size(), 1);
  EXPECT_EQ(a.Peek(), 6);

  a.Push(5);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(a.Peek(), 6);

  EXPECT_EQ(a.Pop(), 6);
  EXPECT_EQ(a.Size(), 1);
  EXPECT_EQ(a.Peek(), 5);

  EXPECT_EQ(a.Pop(), 5);
  EXPECT_EQ(a.Size(), 0);
}

} //namespace ds