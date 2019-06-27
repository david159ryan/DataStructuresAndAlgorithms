#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../string-builder.h"

using ds::StringBuilder;

TEST(StringBuilderTest, Constructor) {
  StringBuilder a;
  StringBuilder b{};

  StringBuilder c{"test", "string"};
  ASSERT_TRUE(c.isEqual({ "test", "string" }));
  ASSERT_EQ(c.Length(), 10);
  ASSERT_STREQ(c[0].c_str(), "test");
  ASSERT_STREQ(c[1].c_str(), "string");
}

TEST(StringBuilderTest, Append) {
  StringBuilder a;

  a.Append("test");
  EXPECT_EQ(a.Length(), 4);

  a.Append("string");
  EXPECT_EQ(a.Length(), 10);
  ASSERT_STREQ(a[0].c_str(), "test");
  ASSERT_STREQ(a[1].c_str(), "string");

  a.Append("");
  EXPECT_EQ(a.Length(), 10);
  ASSERT_STREQ(a[0].c_str(), "test");
  ASSERT_STREQ(a[1].c_str(), "string");

  a.Append("another string");
  EXPECT_EQ(a.Length(), 24);
  ASSERT_STREQ(a[0].c_str(), "test");
  ASSERT_STREQ(a[1].c_str(), "string");
  ASSERT_STREQ(a[2].c_str(), "another string");
}

TEST(StringBuilderTest, Build) {
  StringBuilder a{ "this", "should", "all", "be", "one", "word" };
  ASSERT_STREQ(a.Build().c_str(), "thisshouldallbeoneword");
}
