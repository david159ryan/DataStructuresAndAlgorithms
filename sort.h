#pragma once

#include <iterator>
#include "list.h"

namespace ds {

namespace _ {

template <class T = void>
struct ascending_ {
  template <class T>
  constexpr bool operator()(T&& lhs, T&& rhs) const {
    return lhs < rhs;
  }
};

template <class T = void>
struct descending_ {
  template <class T>
  constexpr bool operator()(T&& lhs, T&& rhs) const {
    return lhs >= rhs;
  }
};

} // namespace _

static auto ascending = _::ascending_<>{};
static auto descending = _::descending_<>{};

template <typename RandomAccessIterator, typename Func>
void merge_sort(RandomAccessIterator first, RandomAccessIterator last, Func& op) {
  size_t distance = std::distance(first, last);
  size_t mid = distance / 2;

  if (distance <= 1) {
    return;
  }

  if (distance == 2) {
    if (!op(*first, *(last - 1))) {
      std::swap(*first, *(last - 1));
    }
    return;
  }

  merge_sort(first, first + mid, op);
  merge_sort(first + mid, last, op);

  auto la = ArrayList<std::remove_reference<decltype(*first)>::type>(distance);

  la.Add(0, first, last);
  auto it = first;

  size_t i = 0;
  size_t j = mid;
  size_t k = 0;

  while (i < mid && j < distance) {
    it[k++] = op(la[i], la[j]) ? std::move(la[i++]) : std::move(la[j++]);
  }

  while (i < mid) {
    it[k++] = la[i++];
  }

  while (j < distance) {
    it[k++] = la[j++];
  }
}

template <typename RandomAccessIterator>
void merge_sort(RandomAccessIterator first, RandomAccessIterator last) {
  merge_sort(first, last, ascending);
}

template <typename RandomAccessIterator, typename Func>
void quick_sort(RandomAccessIterator first, RandomAccessIterator last, Func& op) {
  size_t distance = std::distance(first, last);

  if (distance < 2) return;

  auto pivot = last - 1;

  RandomAccessIterator left = first - 1;
  RandomAccessIterator right = first;

  while (right < pivot) {
    if (op(*right, *pivot)) {
      ++left;
      std::swap(*left, *right);
    }
    ++right;
  }

  std::swap(*pivot, *(left + 1));

  quick_sort(first, left + 1, op);
  quick_sort(left + 2, last, op);
}

template <typename RandomAccessIterator>
void quick_sort(RandomAccessIterator first, RandomAccessIterator last) {
  quick_sort(first, last, ascending);
}

} // namespace ds