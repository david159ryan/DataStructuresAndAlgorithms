#pragma once

#include <memory>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <cstdint>

#include "debug.h"

//using iterator_category = std::forward_iterator_tag;
//using value_type = void; // crap
//using difference_type = void;
//using pointer = void;
//using reference = void;

namespace ds {

// https://hbfs.wordpress.com/2016/03/22/log2-with-c-metaprogramming/
constexpr size_t c_log2(size_t n)
{
  return ((n < 2) ? 1 : 1 + c_log2(n / 2));
}

template <class T>
class ArrayList {
public:
  using value_type = T;
  using const_iterator = const T *;

  static constexpr size_t MAX_CAPACITY = (std::numeric_limits<size_t>::max() >> c_log2(sizeof(T))) - 1;

  ArrayList()
      : count_(0),
        capacity_(0),
        elements_(nullptr) {

  }

  ArrayList(size_t capacity)
      : count_(0),
        capacity_(std::min(MAX_CAPACITY, capacity)),
        elements_(std::make_unique<T[]>(capacity_)) {

  }
  ArrayList(size_t count, const T& repeat)
      : count_(count),
        capacity_(std::min(MAX_CAPACITY, count_)),
        elements_(std::make_unique<T[]>(capacity_)) {
    std::fill(begin(), end(), repeat);
  }


  ArrayList(std::initializer_list<T> const& l)
      : count_(l.size()),
        capacity_(l.size()),
        elements_(std::make_unique<T[]>(capacity_)) {
    std::copy(l.begin(), l.end(), elements_.get());
  }

  // copy constructor
  ArrayList(ArrayList<T> const& other)
      : count_(other.count_),
        capacity_(other.capacity_),
        elements_(std::make_unique<T[]>(capacity_)) {
    std::copy(other.begin(), other.end(), elements_.get());
  }

  // move constructor
  ArrayList(ArrayList<T> && other) noexcept
      : count_(other.count_),
        capacity_(other.capacity_),
        elements_(std::move(other.elements_)) {
    other.count_ = 0;
    other.capacity_ = 0;
    other.elements_.reset();
  }

  ArrayList<T>& operator=(const ArrayList<T>& other) {
    this->count_ = other.count_;
    this->capacity_ = other.capacity_;
    elements_ = std::make_unique<T[]>(capacity_);
    std::copy(other.begin(), other.end(), this->begin());
    return *this;
  }

  ArrayList<T>& operator=(ArrayList<T>&& other) {
    this->count_ = other.count_;
    this->capacity_ = other.capacity_;
    elements_ = std::move(other.elements_);
    other.count_ = 0;
    other.capacity_ = 0;
    other.elements_.reset();
  }
  
  bool operator==(const ArrayList<T>& rhs) const {
    return isEqual(rhs);
  }

  bool isEmpty() const {
    return count_ == 0;
  }

  T* begin() const {
    return elements_.get();
  }

  T* end() const {
    return elements_.get() + count_;
  }

  size_t Size() const {
    return count_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  T Get(size_t index) const {
    check_bounds(index, count_);
    return elements_[index];
  }
  
  template <typename T>
  void Append(T&& val) {
    resize(1);
    elements_[count_ - 1] = std::forward<T>(val);
  }

  T Pop() {
    T val = elements_[count_ - 1];
    Remove(count_ - 1);

    return val;
  }

  void Add(size_t index, const T& val)  {
    // We at most only want to be able to set a value just after the last
    // value, otherwise there would be a gap in the list.
    // e.g. if count is 5, last index is 4, meaning setting 5 is allowed but
    //      setting 6 would leave 5 empty.
    check_bounds(index, count_ + 1);
    shiftElementsRight(index, 1);

    elements_[index] = val;
  }

  void Add(size_t index, T&& val) {
    // We at most only want to be able to set a value just after the last
    // value, otherwise there would be a gap in the list.
    // e.g. if count is 5, last index is 4, meaning setting 5 is allowed but
    //      setting 6 would leave 5 empty.
    check_bounds(index, count_ + 1);
    shiftElementsRight(index, 1);

    elements_[index] = std::move(val);
  }

  template< class InputIt >
  void Add(size_t index, InputIt first, InputIt last) {
    int range = last - first;
    shiftElementsRight(index, range);
    std::copy(first, last, this->begin() + index);
  }

  void Add(size_t index, std::initializer_list<T> const& l) {
    check_bounds(index, count_ + 1);
    Add(index, l.begin(), l.end());
  }

  void Set(size_t index, T&& val) {
    check_bounds(index, count_);
    elements_[index] = val;
  }

  T Remove(size_t index) {
    check_bounds(index, count_);
    T retval = std::move(elements_[index]);
    std::move_backward(this->begin() + index + 1, this->end(), this->end() - 1);
    resize(-1);

    return retval;
  }

  bool isEqual(std::initializer_list<T> const& l) const {
    return std::equal(l.begin(), l.end(), this->begin(), this->end());
  }

  bool isEqual(const ArrayList<T> & other) const {
    return std::equal(other.begin(), other.end(), this->begin(), this->end());
  }

  // Square bracket read, no bounds checking.
  T operator [](size_t i) const {
    return elements_[i];
  }

  // Square bracket assignment, no bounds checking
  T & operator [](size_t i) {
    return elements_[i];
  }

protected:
  size_t count_;
  size_t capacity_;
  std::unique_ptr<T[]> elements_;

private:

  static constexpr size_t DEFAULT_CAPACITY = 0;

  // increase or decrease count and capactiy enough to fit 'diff' more or less elements
  void resize(int diff) {
    size_t old_count = count_;
    count_ = count_ + diff;

    if (count_ == 0) {
      capacity_ = 0;
      elements_.reset(nullptr);
      return;
    }

    if (count_ > capacity_) {
      while (count_ > capacity_) {
        capacity_ = std::max((size_t)1, capacity_ * 2);
      }
    }
    else if (count_ < capacity_ / 2) {
      capacity_ /= 2;
    }
    else {
      return;
    }

    auto newArray = std::make_unique<T[]> (capacity_);
    auto arr_start = newArray.get();

    if (elements_.get()) {
      std::move(this->begin(), this->begin() + old_count, arr_start);
    }
    elements_ = std::move(newArray);
  }

  void shiftElementsRight(size_t index, int amount) {
    assert(amount >= 0);
    resize(amount);
    std::move_backward(this->begin() + index, this->begin() + (count_ - amount), this->end());
  }
};

// Singly Linked List
template <class T>
class SLList {
private:
	class Node {
	private:
		T value_;
    Node * prev_;
    Node * next_;

  public:
    template <typename T,
              typename = std::enable_if_t<std::is_trivially_copyable<T>::value>
    >
		Node(const T& value, Node* next = nullptr, Node* prev = nullptr)
        : value_(value),
          next_(next),
          prev_(prev) {

		}

    template <typename T>
    Node(T&& value, Node* next = nullptr, Node* prev = nullptr)
      : value_(std::move(value)),
        next_(next),
        prev_(prev) {

    }

    ~Node() {
      
    }

    friend class SLList;
	}; // class Node

public:

  class Iterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = T *;
  using reference = T &;
private:
  Node * ptr_;

  Iterator() = delete;

public:

  explicit Iterator(Node * src)
    : ptr_(src) {

  }

  //difference_type operator-(const Iterator& rhs) const {
  //  return std::distance(this->ptr_, rhs.ptr_);
  //}
  Iterator& operator=(const Iterator& rhs) { 
    ptr_ = rhs.ptr_; 
    return *this; 
  }

  reference operator*() { return ptr_->value_; }

  pointer operator&() { return &ptr_->value_; }

  reference operator->() { return ptr_->value_; }
  
  //prefix
  Iterator& operator++() { ptr_ = ptr_->next_; return *this; } 
  
  // postfix
  Iterator operator++(int) { 
    auto temp = ptr_;
    ptr_ = ptr_->next_;
    return Iterator(temp); 
  } 

  bool operator==(const Iterator& rhs) const {
    return this->ptr_ == rhs.ptr_;
  }

  bool operator!=(const Iterator& rhs) const {
    return this->ptr_ != rhs.ptr_;
  }
  //// prefix
  //Iterator& operator--() {
  //  --ptr_;
  //  return *this;
  //}

  //// postfix
  //Iterator operator--(int) {
  //  return *(this + 1);
  //}


  //friend Iterator operator+(const Iterator& lhs, size_t rhs) {
  //  return Make(lhs, rhs);
  //}

  //friend Iterator operator-(const Iterator& lhs, int rhs) {
  //  return Make(lhs, rhs);
  //}
}; // class Iterator

private:
  Node* end_;
  Node* head_;
  Node* tail_;
  size_t count_;

public:

  SLList()
      : end_(new Node(T{})),
        head_(end_),
        tail_(end_),
        count_(0) {

  }

  SLList(const SLList & other)
      : end_(new Node(T{})),
        head_(other.head_),
        tail_(other.tail_),
        count_(other.count_) {

  }

  SLList(SLList&& other)
      : end_(std::move(other.end_)),
        head_(std::move(other.head_)),
        tail_(std::move(other.tail_)),
        count_(std::move(other.count_)) {
    other.end_ = nullptr;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.count_ = 0;
  }

  SLList(std::initializer_list<T> init)
    : end_(new Node(T{})),
      head_(end_),
      tail_(end_),
      count_(0) {
    for (auto val : init) {
      Append(std::forward<T>(val));
    }
  }

  ~SLList() {
    Node* current = head_;

    while (current != end_) {
      Node* next = current->next_;
      delete current;
      current = next;
    }
    if (end_) {
      delete end_;
    }
  }

  Iterator begin() const {
    return Iterator(head_);
  }

  Iterator end() const {
    return Iterator(end_);
  }

  const Iterator cbegin() const {
    return Iterator(head_);
  }

  const Iterator cend() const  {
    return Iterator(end_);
  }

  bool isEmpty() const {
    return count_ == 0;
  }

  size_t Size() const {
    return count_;
  }
  
  T Get(size_t index) const {
    return nodeAt(index)->value_;
  }

  template <typename T>
  void Append(T&& val) {
    if (count_ == 0) {
      head_ = new Node(std::forward<T>(val));
      tail_ = head_;
    }
    else if (count_ == 1) {
      tail_ = new Node(std::forward<T>(val));
      head_->next_ = tail_;
    }
    else {
      tail_->next_ = new Node(std::forward<T>(val));
      tail_ = tail_->next_;
    }
    tail_->next_ = end_;

    ++count_;
  }

  void Set(size_t index, T&& val) {
    nodeAt(index)->value_ = val;
  }

  T Remove(size_t index) {
    Node* before = nodeBefore(index);
    --count_;
    auto retval = (index == 0) ? std::forward<T>(before->value_) : std::forward<T>(before->next_->value_);

    if (index == 0) {
      head_ = std::move(before->next_);
      delete before;
    }
    else if (before->next_ == tail_) {
      delete tail_;
      tail_ = std::move(before);
      tail_->next_ = std::move(end_);
    }
    else {
      Node* next = std::move(before->next_);
      before->next_ = std::move(before->next_->next_);
      delete next;
    }

    return retval;
  }

  bool isEqual(const std::initializer_list<T> & init) {
    return (count_ == init.size()) && (std::equal(this->begin(), this->end(), init.begin()));
  }

private:
  Node* nodeAt(size_t index) const {
    check_bounds(index, count_);
    Node* current = head_;

    for (size_t i = 0; i < index; ++i) {
      current = current->next_;
    }

    return current;
  }

  Node* nodeBefore(size_t index) const {
    check_bounds(index, count_);

    if (index == 0) {
      return head_;
    }

    return nodeAt(index - 1);
  }
};

template <class T>
class ArrayListAppendFunctor {
public:
  ArrayList<T> vec_;

  void operator() (const T& val) {
    vec_.Append(val);
  }
};

} // namespace ds

template <class T>
std::ostream& operator<<(std::ostream& os, const ds::ArrayList<T>& input) {
  if (input.Size() == 0) {
    os << "[]";
    return os;
  }

  os << "[";
  std::copy(input.begin(), input.end() - 1, std::ostream_iterator<T>(os, ","));
  os << *(input.end() - 1) << "]";

  return os;
}


//class Iterator {
//public:
//  using iterator_category = std::random_access_iterator_tag;
//  using value_type = T;
//  using difference_type = ptrdiff_t;
//  using pointer = T *;
//  using reference = T &;
//private:
//  T* ptr_;
//
//  Iterator() = delete;
//  explicit Iterator(T* src, size_t offset = 0)
//    : ptr_(src + offset) {
//
//  }
//
//public:
//  static Iterator Make(T* src, size_t offset = 0);
//
//  static Iterator Make(const Iterator& other, size_t offset = 0);
//
//  Iterator& operator=(const Iterator& rhs) {
//    ptr_ = rhs.ptr_;
//    return *this;
//  }
//
//  difference_type operator-(const Iterator& rhs) const {
//    return std::distance(this->ptr_, rhs.ptr_);
//  }
//
//  reference operator*() {
//    return *ptr_;
//  }
//
//  reference operator->() {
//    return *ptr_;
//  }
//
//  // prefix
//  Iterator& operator++() {
//    ++ptr_;
//    return *this;
//  }
//
//  // postfix
//  Iterator operator++(int) {
//    ptr_++;
//    return *(this - 1);
//  }
//
//  // prefix
//  Iterator& operator--() {
//    --ptr_;
//    return *this;
//  }
//
//  // postfix
//  Iterator operator--(int) {
//    return *(this + 1);
//  }
//
//  bool operator==(const Iterator& rhs) const {
//    return this->ptr_ == rhs.ptr_;
//  }
//
//  bool operator!=(const Iterator& rhs) const {
//    return this->ptr_ != rhs.ptr_;
//  }
//
//  friend Iterator operator+(const Iterator& lhs, size_t rhs) {
//    return Make(lhs, rhs);
//  }
//
//  friend Iterator operator-(const Iterator& lhs, int rhs) {
//    return Make(lhs, rhs);
//  }
//};
//
//template<class T>
//inline typename ArrayList<T>::Iterator ArrayList<T>::Iterator::Make(T* src, size_t offset)
//{
//  return Iterator(src);
//}
//
//template<class T>
//inline typename ArrayList<T>::Iterator ArrayList<T>::Iterator::Make(const typename ArrayList<T>::Iterator& other, size_t offset)
//{
//  return Iterator(other.ptr_, offset);
//}