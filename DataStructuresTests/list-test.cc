#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../list.h"

using ds::ArrayList;
using ds::SLList;
using namespace ::testing;

static void FillList(ArrayList<int> &a, int nums) {
  for (int i = 0; i < nums; ++i) {
    a.Append(i);
  }
}

TEST(ArrayListTest, Constructor) {
  {
    ArrayList<int> a;
    EXPECT_EQ(a.Size(), 0);
    EXPECT_EQ(a.Capacity(), 0);

    ArrayList<int> b;
    EXPECT_EQ(b.Size(), 0);
    EXPECT_EQ(b.Capacity(), 0);

    ArrayList<int> c(10);
    EXPECT_EQ(c.Size(), 0);
    EXPECT_EQ(c.Capacity(), 10);

    ArrayList<int> d{ 3, 2, 1 };
    EXPECT_EQ(d.Size(), 3);
    EXPECT_EQ(d.Capacity(), 3);
    ASSERT_THAT(d, ElementsAreArray({ 3, 2, 1 }));

    ArrayList<int> e(5, 75);
    EXPECT_EQ(e.Size(), 5);
    EXPECT_EQ(e.Capacity(), 5);
    ASSERT_THAT(e, ElementsAreArray({ 75, 75, 75, 75, 75 }));
  }

  {
    // copy constructor
    ArrayList<int> a{ 3, 2, 1 };
    ArrayList<int> b(a);
    EXPECT_EQ(a, b);

    // move constructor
    ArrayList<int> c(std::move(a));
    EXPECT_EQ(c, b);
    EXPECT_EQ(a.Size(), 0);
    EXPECT_EQ(a.Capacity(), 0);
  }
}

TEST(ArrayListTest, isEmpty) {
  ArrayList<int> a;
  EXPECT_TRUE(a.isEmpty());

  a.Append(0);
  EXPECT_FALSE(a.isEmpty());
}

TEST(ArrayListTest, Append) {
  ArrayList<int> a;
  EXPECT_EQ(a.Size(), 0);
  EXPECT_EQ(a.Capacity(), 0);

  a.Append(0);
  EXPECT_EQ(a.Size(), 1);
  EXPECT_EQ(a.Get(0), 0);
  EXPECT_EQ(a.Capacity(), 1);

  a.Append(1);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(a.Get(1), 1);
  EXPECT_EQ(a.Capacity(), 2);

  a.Append(2);
  EXPECT_EQ(a.Size(), 3);
  EXPECT_EQ(a.Get(2), 2);
  EXPECT_EQ(a.Capacity(), 4);

  a.Append(3);
  EXPECT_EQ(a.Size(), 4);
  EXPECT_EQ(a.Get(3), 3);
  EXPECT_EQ(a.Capacity(), 4);

  // ensure all elements copied correctly after resize
  ASSERT_THAT(a, ElementsAreArray({ 0, 1, 2, 3 }));

  ArrayList<int> b(a.Size());
  b.Add(0, a.begin(), a.end());

  EXPECT_TRUE(a.isEqual(b));
}

TEST(ArrayListTest, Add) {
  ArrayList<int> a;

  a.Add(0, 0);
  EXPECT_EQ(a.Size(), 1);
  EXPECT_EQ(a.Get(0), 0);
  EXPECT_EQ(a.Capacity(), 1);

  a.Add(0, 1);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(a.Capacity(), 2);
  EXPECT_TRUE(a.isEqual({ 1, 0 }));

  a.Add(1, 2);
  EXPECT_TRUE(a.isEqual({ 1, 2, 0 }));

  int val = 7;
  a.Add(0, val);
  EXPECT_TRUE(a.isEqual({7, 1, 2, 0 }));
}

TEST(ArrayListTest, Remove) {
  ArrayList<int> a{ 0, 1, 2, 3, 4 };

  EXPECT_EQ(a.Capacity(), 5);

  a.Remove(4);
  EXPECT_EQ(a.Size(), 4);
  EXPECT_EQ(a.Capacity(), 5);
  EXPECT_TRUE(a.isEqual({ 0, 1, 2, 3 }));

  a.Remove(0);
  EXPECT_EQ(a.Capacity(), 5);
  EXPECT_EQ(a.Size(), 3);
  EXPECT_TRUE(a.isEqual({ 1, 2, 3 }));

  a.Remove(1);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_TRUE(a.isEqual({ 1, 3 }));

  a.Remove(0);
  a.Remove(0);

  EXPECT_EQ(a.Size(), 0);
  EXPECT_EQ(a.Capacity(), 0);
}

TEST(ArrayListTest, OutOfBounds) {
  ArrayList<int> a;

  ASSERT_DEATH({ a.Get(0); }, "out of bounds");
  ASSERT_DEATH({ a.Get(3); }, "out of bounds");
  ASSERT_DEATH({ a.Get(-1); }, "out of bounds");

  ASSERT_DEATH({ a.Set(1, 4); }, "out of bounds");
  ASSERT_DEATH({ a.Set(4, 4); }, "out of bounds");
  ASSERT_DEATH({ a.Set(-1, 4); }, "out of bounds");

  ASSERT_DEATH({ a.Set(-1, 4); }, "out of bounds");

  ASSERT_DEATH({ a.Add(1, 1); }, "out of bounds");
  ASSERT_DEATH({ a.Add(1, { 1, 2, 3 }); }, "out of bounds");
}

TEST(ArrayListTest, Pop) {
  ArrayList<int> a{ 0, 1, 2, 3, 4 };

  EXPECT_EQ(a.Capacity(), 5);
  EXPECT_EQ(a.Size(), 5);

  EXPECT_EQ(a.Pop(), 4);
  EXPECT_EQ(a.Capacity(), 5);
  EXPECT_EQ(a.Size(), 4);

  EXPECT_EQ(a.Pop(), 3);
  EXPECT_EQ(a.Capacity(), 5);
  EXPECT_EQ(a.Size(), 3);

  EXPECT_EQ(a.Pop(), 2);  
  EXPECT_EQ(a.Capacity(), 5);
  EXPECT_EQ(a.Size(), 2);

  EXPECT_EQ(a.Pop(), 1);
  EXPECT_EQ(a.Capacity(), 2);
  EXPECT_EQ(a.Size(), 1);

  EXPECT_EQ(a.Pop(), 0);
  EXPECT_EQ(a.Capacity(), 0);
  EXPECT_EQ(a.Size(), 0);

}

TEST(ArrayListTest, SetGet) {
  ArrayList<int> a;
  a.Append(4);
  a.Append(2);
  a.Append(1);

  EXPECT_TRUE(a.isEqual({ 4, 2, 1 }));


  EXPECT_EQ(a.Size(), 3);

  a.Set(0, 0);
  a.Set(1, 1);
  a.Set(2, 2);

  EXPECT_TRUE(a.isEqual({ 0, 1, 2 }));
  EXPECT_EQ(a.Size(), 3);
}

TEST(ArrayListTest, isEqual) {
  ArrayList<int> a{ 0, 1, 2 };
  ArrayList<int> b{ 0, 1, 2 };
  ArrayList<int> c{ 5, 4, 3 };

  EXPECT_TRUE(a.isEqual({ 0,1,2 }));
  EXPECT_TRUE(a.isEqual(b));

  EXPECT_FALSE(a.isEqual({}));
  EXPECT_FALSE(a.isEqual({3, 2, 1}));
  EXPECT_FALSE(a.isEqual(c));
}

TEST(ArrayListTest, AddRange) {
  std::vector<int> v{ 7, 8, 9 };
  ArrayList<int> a{ 0, 1, 2 };

  a.Add(1, v.begin(), v.end());
  EXPECT_EQ(a.Size(), 6);
  EXPECT_TRUE(a.isEqual({ 0,7,8,9,1,2 }));

  a.Add(a.Size(), v.begin(), v.end());
  EXPECT_EQ(a.Size(), 9);
  EXPECT_TRUE(a.isEqual({ 0,7,8,9,1,2,7,8,9 }));
}

TEST(ArrayListTest, InitializerListConstructor) {
  {
    ArrayList<int> a{};
    EXPECT_EQ(a.Size(), 0);
    EXPECT_EQ(a.Capacity(), 0);
  }
  {
    ArrayList<int> a{1};
    EXPECT_EQ(a.Size(), 1);
    EXPECT_EQ(a.Capacity(), 1);
    EXPECT_TRUE(a.isEqual({ 1 }));
  }
  {
    ArrayList<int> a{ 1, 2, 3 };
    EXPECT_EQ(a.Size(), 3);
    EXPECT_EQ(a.Capacity(), 3);
    EXPECT_TRUE(a.isEqual({ 1, 2, 3 }));
  }
  {
    ArrayList<int> a{ 1, 2, 3 };
    EXPECT_FALSE(a.isEqual({ 3, 2, 1 }));
    EXPECT_FALSE(a.isEqual({}));
  }
}

TEST(ArrayListTest, SquareBrackets) {
  ArrayList<int> a{ 3, 2, 1 };

  EXPECT_EQ(a[0], 3);
  EXPECT_EQ(a[1], 2);
  EXPECT_EQ(a[2], 1);

  a[0] = 0;
  a[1] = 1;
  a[2] = 2;

  EXPECT_TRUE(a.isEqual({ 0, 1, 2 }));
}

TEST(ArrayListTest, Move) {
  auto ilist = { 3, 2, 1 };
  ArrayList<int> a{ ilist };
  ArrayList<int> b(std::move(a));
  ArrayList<int> c(ArrayList<int>{ilist});
  ArrayList<int> d(b);

  ASSERT_TRUE(a.isEqual({}));
  ASSERT_TRUE(b.isEqual(ilist));
  ASSERT_TRUE(c.isEqual(ilist));
  ASSERT_TRUE(b.isEqual(c));
  ASSERT_TRUE(b.isEqual(d));
}

TEST(ArrayListTest, StreamOperator) {
  {
    ArrayList<int> a;
    std::stringstream ss;

    ss << a;
    ASSERT_STREQ(ss.str().c_str(), "[]");
  }
  {
    ArrayList<int> a{ 1, 2, 3 };
    std::stringstream ss;

    ss << a;
    ASSERT_STREQ(ss.str().c_str(), "[1,2,3]");
  }
}

TEST(SLListTest, Constructor) {
  SLList<int> a;
  EXPECT_EQ(a.Size(), 0);

  SLList<int> b{ 0, 1, 2 };
  EXPECT_TRUE(b.isEqual({ 0, 1, 2 }));

}

//class DestructorTest {
//private:
//  int val_;
//  static int calls_;
//public:
//
//  DestructorTest(int val = 0) 
//    : val_(val) {
//
//  }
//
//  ~DestructorTest() {
//    val_ = -1;
//    ++calls_;
//  }
//
//  static int DestructorCalls() {
//    return calls_;
//  }
//
//  int Value() {
//    return val_;
//  }
//
//  static void Reset() {
//    calls_ = 0;
//  }
//};
//int DestructorTest::calls_ = 0;

TEST(SLListTest, Append) {
  SLList<int> a;

  a.Append(0);

  EXPECT_EQ(a.Size(), 1);
  EXPECT_EQ(a.Get(0), 0);

  a.Append(1);
  EXPECT_EQ(a.Size(), 2);
  EXPECT_EQ(a.Get(0), 0);
  EXPECT_EQ(a.Get(1), 1);

  a.Append(2);
  a.Append(3);
  a.Append(4);

  EXPECT_EQ(a.Size(), 5);

  EXPECT_TRUE(a.isEqual({ 0, 1, 2, 3, 4 }));
}

TEST(SLListTest, MoveAppend) {
  SLList<std::unique_ptr<int>> a;
  auto ptr = std::make_unique<int>(5);

  a.Append(std::move(ptr));

  EXPECT_EQ(ptr.get(), nullptr);
}

TEST(SLListTest, Remove) {
  SLList<int> a;

  a.Append(7);
  EXPECT_EQ(a.Size(), 1);

  int t = a.Remove(0);

  EXPECT_EQ(t, 7);
  EXPECT_EQ(a.Size(), 0);
  EXPECT_TRUE(a.isEmpty());

  a.Append(6);
  a.Append(5);
  a.Append(4);
  a.Append(3);

  EXPECT_EQ(a.Remove(1), 5);
  EXPECT_TRUE(a.isEqual({6, 4, 3}));
  EXPECT_EQ(a.Remove(1), 4);
  EXPECT_TRUE(a.isEqual({ 6, 3 }));
  EXPECT_EQ(a.Remove(0), 6);
  EXPECT_TRUE(a.isEqual({ 3 }));
  EXPECT_EQ(a.Remove(0), 3);
  EXPECT_TRUE(a.isEqual({}));
}
