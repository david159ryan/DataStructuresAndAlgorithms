#pragma once

#include "list.h"

namespace ds {

template <class T>
class Queue : private SLList<T> {

public:
  Queue()
    : SLList<T>() {

  }

  size_t Size() {
    return SLList<T>::Size();
  }

  void Push(const T& val) {
    SLList<T>::Append(val);
  }

  void Push(T&& val) {
    SLList<T>::Append(std::move(val));
  }

  T Pop() {
    return SLList<T>::Remove(0);
  }

  T Peek() {
    return SLList<T>::Get(0);
  }

  bool isEmpty() {
    return SLList<T>::isEmpty();
  }
};

} // namespace ds