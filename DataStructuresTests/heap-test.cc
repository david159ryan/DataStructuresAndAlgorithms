#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../heap.h"

namespace ds {
using namespace ::testing;

TEST(HeapTest, MinHeap) {
  BinHeap<int, MinHeap> h{ 7, 9, 4, 0, 11, 3, 1, 2 };

  ASSERT_EQ(h.Peek(), 0);
}

TEST(HeapTest, MaxHeap) {
  BinHeap<int, MaxHeap> h{ 7, 9, 4, 0, 11, 3, 1, 2 };

  ASSERT_EQ(h.Peek(), 11);
}

TEST(HeapTest, Insert) {
  BinHeap<int, MinHeap> h;

  h.Insert(4);
  ASSERT_EQ(h.Peek(), 4);

  h.Insert(5);
  ASSERT_EQ(h.Peek(), 4);

  h.Insert(2);
  ASSERT_EQ(h.Peek(), 2);
}

TEST(HeapTest, Pop) {
  BinHeap<int, MinHeap> h{ 7, 9, 4, 0, 11, 3, 1, 2 };
  ArrayList<int> a;
  
  while (!h.isEmpty()) {
    a.Append(h.Pop());
  }

  ASSERT_THAT(a, ElementsAreArray({ 0, 1, 2, 3, 4, 7, 9, 11 }));
}

} // namespace ds