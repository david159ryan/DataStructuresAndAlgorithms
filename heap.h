#pragma once

#include <functional>

#include "list.h"
#include <array>
namespace ds {

namespace _ {
struct HeapType {};
} // namespace _

struct MinHeap : _::HeapType {};
struct MaxHeap : _::HeapType {};

template <class T, class H, 
  typename = std::enable_if_t<std::is_base_of<_::HeapType, H>::value>
>
class BinHeap : protected ArrayList<T> {

public:
  using HeapType = H;

  BinHeap()
      : ArrayList<T>() {

  }

  BinHeap(size_t capacity)
      : ArrayList<T>(capacity) {

  }

  BinHeap(std::initializer_list<T>&& in) {
    for (auto val : in) {
      Insert(val);
    }
  }

  bool isEmpty() const {
    return ArrayList<T>::isEmpty();
  }

  void Insert(T val) {
    ArrayList<T>::Append(val);
    fixUp(ArrayList<T>::Size() - 1);
  }

  T Peek() const {
    return ArrayList<T>::Get(0);
  }

  T Pop() {
    if (count_ == 1) {
      return ArrayList<T>::Pop();
    }

    std::swap(elements_[0], elements_[count_ - 1]);
    auto retval = ArrayList<T>::Pop();
    heapify(0);
    return retval;
  }

private:

  bool compare(size_t lhs, size_t rhs) const {
    return std::is_same<HeapType, MinHeap>::value ? elements_[lhs] < elements_[rhs] : elements_[lhs] > elements_[rhs];
  }

  size_t parentIndex(size_t index) const {
    return (index - 1) / 2;
  }

  size_t leftChildIndex(size_t index) const {
    return ((index * 2) + 1);
  }

  size_t rightChildIndex(size_t index) const {
    return ((index * 2) + 2);
  }

  T parent(size_t index) const {
    return elements_[parentIndex(index)];
  }

  bool hasLeft(size_t index) {
    return leftChildIndex(index) < count_;
  }

  bool hasRight(size_t index) {
    return rightChildIndex(index) < count_;
  }

  T leftChild(size_t index) const {
    return elements_[leftChildIndex(index)];
  }

  T rightChild(size_t index) const {
    return elements_[rightChildIndex(index)];
  }

  size_t fixUp(size_t index) {
    size_t current = index;

    while (current != 0 && compare(current, parentIndex(current))) {
      std::swap(elements_[current], elements_[parentIndex(current)]);
      current = parentIndex(current);
    }

    return current;
  }

  void heapify(size_t current) {
    size_t priority = current;

    if (hasLeft(current)) {
      priority = compare(priority, leftChildIndex(current)) ? priority : leftChildIndex(current);
    }

    if (hasRight(current)) {
      priority = compare(priority, rightChildIndex(current)) ? priority : rightChildIndex(current);
    }

    if (priority != current) {
      std::swap(elements_[current], elements_[priority]);
      heapify(priority);
    }
  }
};

} // namespace ds