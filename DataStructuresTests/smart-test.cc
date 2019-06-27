#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../smart.h"

using namespace ::testing;
TEST(SmartTest, Constructor) {
  ds::ref_ptr<int> ref(new int(5));
  EXPECT_EQ(*ref, 5);
  EXPECT_EQ(ref.ref_count(), 1);
  
  {
    ds::ref_ptr<int> ref2(ref);
    EXPECT_EQ(*ref2, 5);
    EXPECT_EQ(ref.ref_count(), 2);
    EXPECT_EQ(ref2.ref_count(), 2);
  }

  EXPECT_EQ(ref.ref_count(), 1);
}

TEST(SmartTest, FromExistingPointer) {
  int* val = new int(5);
  ds::ref_ptr<int> ref(val);
  EXPECT_EQ(*ref, 5);
  EXPECT_EQ(ref.ref_count(), 1);

  {
    ds::ref_ptr<int> ref2(val);
    EXPECT_EQ(*ref2, 5);
    EXPECT_EQ(ref.ref_count(), 2);
    EXPECT_EQ(ref2.ref_count(), 2);
  }
  EXPECT_EQ(ref.ref_count(), 1);
}

TEST(SmartTest, make_ref) {
  auto ref = ds::make_ref_ptr<int>(6);
  EXPECT_EQ(*ref, 6);
  EXPECT_EQ(ref.ref_count(), 1);

  auto ref_vec = std::make_unique<std::vector<int>>( 4, 4 );
  ASSERT_THAT(*ref_vec, ElementsAreArray({ 4, 4, 4, 4 }));
  
}

TEST(SmartTest, move) {
  auto ref = ds::make_ref_ptr<int>(6);
  auto moved = std::move(ref);

  EXPECT_EQ(ref, nullptr);
  EXPECT_EQ(*moved, 6);
  EXPECT_EQ(moved.ref_count(), 1);
  EXPECT_EQ(ref.ref_count(), 0);
}