#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../table.h"

namespace ds {

TEST(HashTableTest, Constructor) {
  {
    HashTable<std::string, int> table;
    EXPECT_EQ(table.Size(), 0);
    EXPECT_TRUE(table.isEmpty());
  }
  {
    HashTable<std::string, int> table{ std::make_pair("key0", 0) };
    EXPECT_EQ(table.Size(), 1);
    EXPECT_FALSE(table.isEmpty());
  }
  {
    HashTable<std::string, int> table{ 
      {"key0", 0},
      {"key1", 1},
      {"key1", 2}
    };
    EXPECT_EQ(table.Size(), 2);
    EXPECT_FALSE(table.isEmpty());
  }
  //{
  //  auto entry = std::make_unique<int>(5);
  //  auto pair = std::make_pair(4, std::move(entry));
  //  HashTable<int, std::unique_ptr<int>> table{ std::move(pair) };
  //  EXPECT_EQ(table.Size(), 1);
  //  EXPECT_FALSE(table.isEmpty());
  //}
}

TEST(HashTableTest, Insert) {
  auto pair = std::make_pair(4, std::make_unique<int>(5));
  HashTable<int, std::unique_ptr<int>> table;

  EXPECT_TRUE(table.Insert(std::move(pair)));
  EXPECT_EQ(table.Size(), 1);
  EXPECT_FALSE(table.isEmpty());

  EXPECT_FALSE(table.Insert(std::move(pair)));
  EXPECT_EQ(table.Size(), 1);

  table.Insert(std::make_pair(5, std::make_unique<int>(3)));
  EXPECT_EQ(table.Size(), 2);
}

TEST(HashTableTest, Find) {
  HashTable<std::string, int> table{
    {"key0", 3},
    {"key1", 4},
    {"key2", 5}
  };
  EXPECT_EQ(table.Size(), 3);

  EXPECT_EQ((**table.Find("key0")), 3);
  EXPECT_EQ((**table.Find("key1")), 4);
  EXPECT_EQ((**table.Find("key2")), 5);

  EXPECT_FALSE(table.Insert(std::make_pair( "key2", 7 )));
  EXPECT_EQ((**table.Find("key2")), 7);
  EXPECT_EQ(table.Size(), 3);

  EXPECT_EQ((table.Find("not found")), std::nullopt);
}

TEST(HashTableTest, Remove) {
  HashTable<std::string, int> table{
    {"key0", 3},
    {"key1", 4},
    {"key2", 5}
  };
  EXPECT_EQ(table.Size(), 3);

  EXPECT_EQ(table.Remove("key0"), 3);
  EXPECT_EQ(table.Size(), 2);
  EXPECT_EQ((table.Find("key0")), std::nullopt);

  EXPECT_EQ(table.Remove("key1"), 4);
  EXPECT_EQ(table.Size(), 1);
  EXPECT_EQ((table.Find("key0")), std::nullopt);

  EXPECT_EQ(table.Remove("key2"), 5);
  EXPECT_EQ(table.Size(), 0);
  EXPECT_EQ((table.Find("key0")), std::nullopt);

  EXPECT_TRUE(table.isEmpty());
}

} // namespace ds
