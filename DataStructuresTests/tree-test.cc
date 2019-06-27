#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../list.h"
#include "../tree.h"


namespace ds {

TEST(AVLTreeTest, Constructor) {
  {
    auto tree = AVLTree<int>::NewRoot(5);
    EXPECT_EQ(tree->Height(), 1);
    EXPECT_TRUE(tree->isRoot());
  }
  {
    auto tree = AVLTree<int>::NewRoot(5);
    EXPECT_EQ(tree->Height(), 1);
    EXPECT_TRUE(tree->isRoot());
  }
}

TEST(AVLTreeTest, Insert) {
  auto tree = AVLTree<int>::NewRoot(5);
  EXPECT_EQ(tree->Height(), 1);
  EXPECT_TRUE(tree->isRoot());

  auto inserted = tree->Insert(7);
  EXPECT_EQ(inserted->Value(), 5);

  // Insertion returns root
  EXPECT_TRUE(inserted->isRoot());
  EXPECT_EQ(tree->Height(), 2);

  EXPECT_EQ(tree->Insert(4)->Value(), 5);
  EXPECT_EQ(tree->Height(), 2);

  EXPECT_EQ(tree->Insert(8)->Value(), 5);
  EXPECT_EQ(tree->Height(), 3);
}

TEST(AVLTreeTest, RotateRight) {
  auto tree = AVLTree<int>::NewRoot(5);
  EXPECT_EQ(tree->Insert(4)->Value(), 5);
  EXPECT_EQ(tree->Insert(3)->Value(), 4);
  EXPECT_EQ(tree->Insert(2)->Value(), 4);
  EXPECT_EQ(tree->Insert(1)->Value(), 4);
  EXPECT_EQ(tree->Insert(0)->Value(), 2);
  EXPECT_EQ(tree->Height(), 3);
}

TEST(AVLTreeTest, RotateLeft) {
  auto tree = AVLTree<int>::NewRoot(1);
  EXPECT_EQ(tree->Insert(2)->Value(), 1);
  EXPECT_EQ(tree->Insert(3)->Value(), 2);
  EXPECT_EQ(tree->Insert(4)->Value(), 2);
  EXPECT_EQ(tree->Insert(5)->Value(), 2);
  EXPECT_EQ(tree->Insert(6)->Value(), 4);
  EXPECT_EQ(tree->Height(), 3);
}

TEST(AVLTreeTest, RotateLeftRight) {
  auto tree = AVLTree<int>::NewRoot(4);
  EXPECT_EQ(tree->Insert(5)->Value(), 4);
  EXPECT_EQ(tree->Insert(1)->Value(), 4);
  EXPECT_EQ(tree->Insert(0)->Value(), 4);
  EXPECT_EQ(tree->Insert(3)->Value(), 4);
  EXPECT_EQ(tree->Insert(2)->Value(), 3);
  EXPECT_EQ(tree->Height(), 3);
}

TEST(AVLTreeTest, RotateRightLeft) {
  auto tree = AVLTree<int>::NewRoot(1);
  EXPECT_EQ(tree->Insert(0)->Value(), 1);
  EXPECT_EQ(tree->Insert(4)->Value(), 1);
  EXPECT_EQ(tree->Insert(3)->Value(), 1);
  EXPECT_EQ(tree->Insert(2)->Value(), 1);
  EXPECT_EQ(tree->Insert(5)->Value(), 3);
  EXPECT_EQ(tree->Height(), 3);
}

TEST(AVLTreeTest, Find) {
  auto tree = AVLTree<int>::FromList({ 0, 1, 2, 3 });
  ASSERT_TRUE(tree->Find(0).has_value());
  ASSERT_EQ(tree->Find(0).value()->Value(), 0);
  ASSERT_TRUE(tree->Find(1).has_value());
  ASSERT_EQ(tree->Find(1).value()->Value(), 1);
  ASSERT_TRUE(tree->Find(2).has_value());
  ASSERT_EQ(tree->Find(2).value()->Value(), 2);
  ASSERT_TRUE(tree->Find(3).has_value());
  ASSERT_EQ(tree->Find(3).value()->Value(), 3);
  ASSERT_FALSE(tree->Find(4).has_value());
  ASSERT_FALSE(tree->Find(-1).has_value());
}

TEST(AVLTreeTest, FindMin) {
  auto tree = AVLTree<int>::FromList({ 0, 1, 2, 3 });
  ASSERT_EQ(tree->FindMin().value()->Value(), 0);
}

TEST(AVLTreeTest, FindRoot) {
  auto tree = AVLTree<int>::FromList({ 0, 1, 2, 3 });
  auto root = tree->FindMin().value()->FindRoot().value();
  ASSERT_EQ(root->Value(), 1);
  ASSERT_TRUE(root->isRoot());
}

TEST(AVLTreeTest, Remove) {
  auto tree = AVLTree<int>::FromList({ 0, 1, 2, 3 });
  ASSERT_EQ(tree->Height(), 3);
  ASSERT_EQ(tree->Value(), 1);
  tree = tree->Remove(3);
  ASSERT_EQ(tree->Height(), 2);
  ASSERT_EQ(tree->Value(), 1);
  tree = tree->Remove(0);
  ASSERT_EQ(tree->Height(), 2);
  ASSERT_EQ(tree->Value(), 1);
  tree = tree->Remove(1);
  ASSERT_EQ(tree->Height(), 1);
  ASSERT_EQ(tree->Value(), 2);
  tree = tree->Remove(2);
  ASSERT_FALSE(tree);
}

TEST(AVLTreeTest, PreorderTraversal) {
  ArrayListAppendFunctor<int> v;
  auto tree = AVLTree<int>::FromList({ 0, 1, 2, 3, 4 });
  tree->PreorderTraversal(v);
  auto expected = { 1, 0, 3, 2, 4 };
  EXPECT_TRUE(v.vec_.isEqual(expected));
}

TEST(AVLTreeTest, InorderTraversal) {
  ArrayListAppendFunctor<int> v;
  auto tree = AVLTree<int>::FromList({0, 1, 2, 3, 4});
  tree->InorderTraversal(v);
  auto expected = { 0, 1, 2, 3, 4 };
  EXPECT_TRUE(v.vec_.isEqual(expected));
}

TEST(AVLTreeTest, PostorderTraversal) {
  ArrayListAppendFunctor<int> v;
  auto tree = AVLTree<int>::FromList({ 0, 1, 2, 3, 4 });
  tree->PostorderTraversal(v);
  auto expected = { 0, 2, 4, 3, 1 };
  EXPECT_TRUE(v.vec_.isEqual(expected));
}

TEST(Trie, Test) {
  Trie t;
  t.insert("cat");
  t.insert("catatonic");
  t.insert("dog");
  t.insert("ball");
  t.insert("baseball");

  ASSERT_TRUE(t.find("cat"));
  ASSERT_TRUE(t.find("catatonic"));
  ASSERT_TRUE(t.find("dog"));
  ASSERT_TRUE(t.find("ball"));
  ASSERT_TRUE(t.find("baseball"));
  ASSERT_TRUE(t.find("base", true)); // exists as a substring

  ASSERT_FALSE(t.find("base")); // doesn't exists as a full word
  ASSERT_FALSE(t.find("can"));
  ASSERT_FALSE(t.find("zork"));
}

} // namespace ds