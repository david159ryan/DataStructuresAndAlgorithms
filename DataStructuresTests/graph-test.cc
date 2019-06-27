#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../graph.h"

namespace ds {
using namespace ::testing;

TEST(GraphTest, Constructor) {
  AdjacencyListGraph<int> g(3, { 0, 10, 20 },
    { {0, 1, 1}, {0, 2, 1}, {1, 2, 1}, {2, 0, 1} }
  );

  ASSERT_EQ(g.NodeCount(), 3);
  ASSERT_EQ(g.EdgeCount(), 4);
}

TEST(GraphTest, DFS) {
  AdjacencyListGraph<int> g(4, { 0, 10, 20, 30 },
    { {0, 1, 1}, {0, 3, 1}, {3, 2, 1}, {2, 1, 1}, {1, 3, 1}
  });
  ArrayListAppendFunctor<size_t> v;

  g.DFS(0, v);
  ASSERT_THAT(v.vec_, ElementsAreArray({ 0, 1, 3, 2 }));
}

TEST(GraphTest, BFS) {
  AdjacencyListGraph<int> g(6, { 0, 10, 20, 30, 40, 50 },
    { {0, 1, 1}, {0, 2, 1}, {2, 4, 1}, {4, 5, 1}, {4, 3, 1 }
  });
  ArrayListAppendFunctor<size_t> v;

  auto path = g.BFS(0, 5, v);
  ASSERT_THAT(v.vec_, ElementsAreArray({ 0, 1, 2, 4, 5 }));
  ASSERT_THAT(path, ElementsAreArray({ 0, 2, 4, 5 }));
}

TEST(GraphTest, BFS_NoPath) {
  AdjacencyListGraph<int> g(6, { 0, 10, 20, 30, 40, 50 },
    { {0, 1, 1}, {0, 2, 1}, {2, 4, 1}, {4, 3, 1 }
    });
  ArrayListAppendFunctor<size_t> v;

  auto path = g.BFS(0, 5, v);
  ASSERT_THAT(v.vec_, ElementsAreArray({ 0, 1, 2, 4, 3 }));
  ASSERT_TRUE(path.isEmpty());
}

TEST(GraphTest, Dikstras) {
  {
    AdjacencyListGraph<int> g(6, { 0, 0, 0, 0, 0, 0 }, {
      {0, 1, 2}, {0, 2, 3}, {2, 4, 5}, {2, 3, 10}, {4, 3, 1 }
    });

    auto path = g.Dikstras(0, 3);
    ASSERT_THAT(path.first, ElementsAreArray({ 0, 2, 4, 3 }));
    EXPECT_NEAR(path.second, 9.0, 0.1);
  }
  {
    AdjacencyListGraph<int> g(6, { 0, 0, 0, 0, 0, 0 }, {
      {0, 1, 2}, {0, 2, 3}, {2, 4, 5}, {2, 3, 10}, {4, 3, 1 }
    });

    auto path = g.Dikstras(0, 5);
    EXPECT_TRUE(path.first.isEmpty());
    EXPECT_NEAR(path.second, std::numeric_limits<double>::max(), 0.1);
  }
}

} // namespace ds