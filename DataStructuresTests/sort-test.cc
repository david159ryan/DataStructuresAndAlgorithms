#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../sort.h"
#include "../list.h"
namespace ds {
using namespace ::testing;

template <class T, class Func>
constexpr void TEST_SORT(std::initializer_list<T>&& in, std::initializer_list<T>&& res, Func& op) {
  ArrayList<T> a{ std::move(in) };
  merge_sort(a.begin(), a.end(), op);
  ASSERT_THAT(a, ElementsAreArray(res));
}

template <class T>
constexpr void TEST_SORT(std::initializer_list<T>&& in, std::initializer_list<T>&& res) {
  TEST_SORT(std::move(in), std::move(res), ascending);
}

template <class T, class Func>
constexpr void TEST_QSORT(std::initializer_list<T>&& in, std::initializer_list<T>&& res, Func& op) {
  ArrayList<T> a{ std::move(in) };
  quick_sort(a.begin(), a.end(), op);
  ASSERT_THAT(a, ElementsAreArray(res));
}

template <class T>
constexpr void TEST_QSORT(std::initializer_list<T>&& in, std::initializer_list<T>&& res) {
  TEST_QSORT(std::move(in), std::move(res), ascending);
}

TEST(MergeSortTest, ArrayList) {
  TEST_SORT(std::initializer_list<int>{}, {});
  TEST_SORT({ 0 }, { 0 });
  TEST_SORT({ 0, 1 }, { 0, 1 });
  TEST_SORT({ 1, 0 }, { 0, 1 });
  TEST_SORT({ 0, 1, 2 }, { 0, 1, 2 });
  TEST_SORT({ 0, 2, 1 }, { 0, 1, 2 });
  TEST_SORT({ 1, 0, 2 }, { 0, 1, 2 });
  TEST_SORT({ 2, 1, 0 }, { 0, 1, 2 });
  TEST_SORT({ 2, 0, 1 }, { 0, 1, 2 });
  TEST_SORT({ 0, 1, 2, 3 }, { 0, 1, 2, 3 });
  TEST_SORT({ 0, 1, 3, 2 }, { 0, 1, 2, 3 });
  TEST_SORT({ 0, 3, 1, 2 }, { 0, 1, 2, 3 });
  TEST_SORT({ 0, 3, 1, 2 }, { 0, 1, 2, 3 });
  TEST_SORT({ 3, 2, 1, 0 }, { 0, 1, 2, 3 });
  TEST_SORT({ 0, 3, 1, 2, 7, 12, -2, 1, 2, 6 }, { -2, 0, 1, 1, 2, 2, 3, 6, 7, 12 });
  TEST_SORT<std::string>({ "b", "a", "ab", "c", "cab" }, { "a", "ab", "b", "c", "cab" });
}

TEST(MergeSortTest, ArrayListDescending) {
  TEST_SORT({ 0, 3, 1, 2 }, { 3, 2, 1, 0 }, descending);
  TEST_SORT({ 0, 3, 1, 2, 7, 12, -2, 1, 2, 6 }, {12, 7, 6, 3, 2, 2, 1, 1, 0, -2}, descending);
  TEST_SORT<std::string>({ "b", "a", "ab", "c", "cab" }, { "cab", "c", "b", "ab", "a" }, descending);
}

TEST(QuickSortTest, ArrayList) {
  TEST_QSORT(std::initializer_list<int>{}, {});
  TEST_QSORT({ 0 }, { 0 });
  TEST_QSORT({ 0, 1 }, { 0, 1 });
  TEST_QSORT({ 1, 0 }, { 0, 1 });
  TEST_QSORT({ 0, 1, 2 }, { 0, 1, 2 });
  TEST_QSORT({ 0, 2, 1 }, { 0, 1, 2 });
  TEST_QSORT({ 1, 0, 2 }, { 0, 1, 2 });
  TEST_QSORT({ 2, 1, 0 }, { 0, 1, 2 });
  TEST_QSORT({ 2, 0, 1 }, { 0, 1, 2 });
  TEST_QSORT({ 0, 1, 2, 3 }, { 0, 1, 2, 3 });
  TEST_QSORT({ 0, 1, 3, 2 }, { 0, 1, 2, 3 });
  TEST_QSORT({ 0, 3, 1, 2 }, { 0, 1, 2, 3 });
  TEST_QSORT({ 0, 3, 1, 2 }, { 0, 1, 2, 3 });
  TEST_QSORT({ 3, 2, 1, 0 }, { 0, 1, 2, 3 });
  TEST_QSORT({ 1, 2, 2, 0 }, { 0, 1, 2, 2 });
  TEST_QSORT({ 3, 2, 4, 1, 0 }, { 0, 1, 2, 3, 4 });
  TEST_QSORT({ 0, 3, 1, 2, 7, 12, -2, 1, 2, 6 }, { -2, 0, 1, 1, 2, 2, 3, 6, 7, 12 });
  TEST_QSORT<std::string>({ "b", "a", "ab", "c", "cab" }, { "a", "ab", "b", "c", "cab" });
}

TEST(QuickSortTest, ArrayListDescending) {
  TEST_QSORT({ 0, 3, 1, 2 }, { 3, 2, 1, 0 }, descending);
  TEST_QSORT({ 0, 3, 1, 2, 7, 12, -2, 1, 2, 6 }, { 12, 7, 6, 3, 2, 2, 1, 1, 0, -2 }, descending);
  TEST_QSORT<std::string>({ "b", "a", "ab", "c", "cab" }, { "cab", "c", "b", "ab", "a" }, descending);
}

} // namespace ds