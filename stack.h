#pragma once

#include "list.h"

namespace ds {

template <class T>
class Stack {
public:
  virtual ~Stack(){};
  virtual bool isEmpty();
  virtual void Push(const T& val) = 0;
  virtual void Push(T&& val) = 0;
  virtual T Pop();
  virtual const T Peek();
};

} //namespace ds
