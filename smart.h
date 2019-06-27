#pragma once

#include <map>
#include <memory>

namespace ds {

template <class T>
class ref_ptr {
private:

  static std::map<T const*, int> ref_map;

  T* ptr_;

public:
  ref_ptr()
      : ptr_(nullptr) {

  }

  ~ref_ptr() {
    if (ptr_ == nullptr) return;

    auto it = ref_map.find(ptr_);
    it->second--;
    if (it->second == 0) {
      ref_map.erase(ptr_);
      delete ptr_;
      ptr_ = nullptr;
    }
  }

  ref_ptr(T*&& val)
      : ptr_(std::move(val)) {
    auto it = ref_map.find(ptr_);
    if (it != ref_map.end()) {
      it->second++;
    }
    else {
      ref_map.insert(std::make_pair(ptr_, 1));
    }
  }

  ref_ptr(const T* const& val)
      : ptr_(const_cast<T*>(val)) {
    auto it = ref_map.find(ptr_);
    if (it != ref_map.end()) {
      it->second++;
    }
    else {
      ref_map.insert(std::make_pair(ptr_, 1));
    }
  }

  ref_ptr(const ref_ptr& other)
      : ptr_(const_cast<T*>(other.ptr_)) {
    ref_map.find(ptr_)->second++;
  }

  ref_ptr(ref_ptr&& other) noexcept
      : ptr_(std::move(other.ptr_)) {
    other.ptr_ = nullptr;
  }

  int ref_count() const {
    return ptr_ == nullptr ? 0 : ref_map.find(ptr_)->second;
  }

  T& operator*() {
    return *ptr_;
  }

  T& operator->() {
    return *ptr_;
  }

  bool operator==(nullptr_t& null) const {
    return ptr_ == nullptr;
  }

  bool operator==(const ref_ptr& other) const {
    return ptr_ == other.ptr_;
  }
};

template <class T>
std::map<T const*, int> ref_ptr<T>::ref_map{};

template <class T, typename... Args>
ref_ptr<T> make_ref_ptr(Args&& ... args) {
  return new T( std::forward<Args>(args)... );
}


} // namespace ds