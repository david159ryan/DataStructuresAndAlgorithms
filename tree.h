#pragma once

#include <memory>
#include <optional>
#include <string>
#include <map>
#include <unordered_map>
#include "common.h"

namespace ds {

template <class T>
class AVLTree : public std::enable_shared_from_this<AVLTree<T>> {

private:
  size_t height_;
  T value_;
  std::weak_ptr<AVLTree> parent_;
  std::shared_ptr<AVLTree> left_;
  std::shared_ptr<AVLTree> right_;

public:

  AVLTree(const AVLTree& other) = delete;
  AVLTree& operator=(const AVLTree& other) = delete;
  ~AVLTree() {

  }

  static std::shared_ptr<AVLTree> NewRoot(T&& val) {
    return std::shared_ptr<AVLTree>(new AVLTree(std::forward<T>(val)));
  }

  static std::shared_ptr<AVLTree> FromList(std::initializer_list<T>&& list) {
    auto root = std::shared_ptr<AVLTree>(new AVLTree(std::move((*list.begin()))));
    std::for_each(list.begin() + 1, list.end(), [&root](auto val) {
      auto _ = root->Insert(std::move(val));
    });
    return root;
  }

private:
  AVLTree() {};

  template <typename T>
  AVLTree(T&& val)
    : value_(std::forward<T>(val)),
      height_(1),
      parent_(),
      left_(nullptr),
      right_(nullptr) {

  }

  template <typename T>
  AVLTree(T&& val, std::shared_ptr<AVLTree>& parent)
    : value_(std::forward<T>(val)),
      height_(1),
      parent_(parent),
      left_(nullptr),
      right_(nullptr) {

  }

  AVLTree(AVLTree&& other)
    : value_(std::move(other.value_)),
      height_(other.height_),
      parent_(std::move(other.parent_)),
      left_(std::move(other.left_)),
      right_(std::move(other.right_)) {
    other.value_ = T();
    other.height_ = 0;
    other.parent_.reset();
    other.left_.reset();
    other.right_.reset();
  }


  AVLTree& operator=(AVLTree&& other) {
    value_ = std::move(other.value_);
    height_ = other.height_;
    parent_ = std::move(other.parent_);
    left_ = std::move(other.left_);
    right_ = std::move(other.right_);
      
    other.value_ = T();
    other.height_ = 0;
    other.parent_.reset();
    other.left_.reset();
    other.right_.reset();
  }

public:

  size_t Height() const {
    return height_;
  }

  T Value() const {
    return value_;
  }

  bool isRoot() {
    return parent_.expired();
  }
  
  std::shared_ptr<AVLTree> UpdateHeight() {
    size_t l_height = (left_) ? left_->height_ : 0;
    size_t r_height = (right_) ? right_->height_ : 0;

    if (abs_diff(l_height, r_height) >= 2) {
      if (l_height > r_height) {
        if (left_->left_ > left_->right_) {
          // left left
          rotateRight();
        }
        else {
          // left right
          left_->rotateLeft();
          rotateRight();
        }
      }
      else {
        if (right_->right_ > right_->left_) {
          // right right
          rotateLeft();
        }
        else {
          // right left
          right_->rotateRight();
          rotateLeft();
        }
      }
    }
    else {
      height_ = 1 + std::max(l_height, r_height);
    }

    if (parent_.lock()) {
      return parent_.lock()->UpdateHeight();
    }

    return shared_from_this();
  }

  template <typename T>
  std::shared_ptr<AVLTree> Insert(T&& val) {
    std::shared_ptr<AVLTree> * node;

    if (val < value_) {
      if (left_) {
        return left_->Insert(std::forward<T>(val));
      }
      else {
        node = &left_;
      }
    }
    else if (val > value_) {
      if (right_) {
        return right_->Insert(std::forward<T>(val));
      }
      else {
        node = &right_;
      }
    }
    else {
      return shared_from_this();
    }

    (*node).reset(new AVLTree(std::forward<T>(val)));
    (*node)->parent_ = shared_from_this();

    return UpdateHeight();
  }

  std::optional<std::shared_ptr<AVLTree>> Find(const T& val) {
    if (val == value_) {
      return shared_from_this();
    }

    if (val < value_ && left_) {
      return left_->Find(val);
    }
    else if (val > value_ && right_) {
      return right_->Find(val);
    }

    return {};
  }

  std::optional<std::shared_ptr<AVLTree>> FindMin() {
    if (left_) return left_->FindMin();

    return shared_from_this();
  }

  std::optional<std::shared_ptr<AVLTree>> FindRoot() {
    if (isRoot()) {
      return shared_from_this();
    }

    return parent_.lock()->FindRoot();
  }

  std::shared_ptr<AVLTree> Remove(const T& val) {
    if (val < value_ && left_) {
      return left_->Remove(val);
    }
    
    if (val > value_ && right_) {
      return right_->Remove(val);
    }
    
    if (val == value_) {
      // 0 or 1 child
      if (!left_ || !right_) {
        auto temp = left_ ? std::move(left_) : std::move(right_);
        
        if (temp) {
          // 1 child
          value_ = std::move(temp->value_);
          left_ = std::move(temp->left_);
          right_ = std::move(temp->right_);
          return UpdateHeight();
        }
        else {
          // leaf node
          auto parent = parent_.lock();
          if (parent) {
            if (parent->value_ < val) {
              parent->right_.reset();
            }
            else {
              parent->left_.reset();
            }
            return parent->UpdateHeight();
          }
          else {
            // remove last node
            return {};
          }
        }
      }
      else {
        // 2 children
        auto temp = std::move(right_->FindMin().value());
        value_ = std::move(temp->value_);
        auto parent = temp->parent_.lock();
        parent->left_.reset();
        return parent->UpdateHeight();
      }
    }

    return FindRoot().value();
  }
  
  void Clear() {
    delete this;
  }

  friend bool operator >(const std::shared_ptr<AVLTree<T>>& lhs, const std::shared_ptr<AVLTree<T>>& rhs) {
    size_t l_height = (lhs) ? lhs->height_ : 0;
    size_t r_height = (rhs) ? rhs->height_ : 0;

    return l_height > r_height;
  }

  template <typename Func>
  void PreorderTraversal(Func& f) {
    f(value_);
    if (left_) {
      left_->PreorderTraversal(f);
    }
    if (right_) {
      right_->PreorderTraversal(f);
    }
  }

  template <typename Func>
  void InorderTraversal(Func& f) {
    if (left_) {
      left_->InorderTraversal(f);
    }
    f(value_);
    if (right_) {
      right_->InorderTraversal(f);
    }
  }

  template <typename Func>
  void PostorderTraversal(Func& f) {
    if (left_) {
      left_->PostorderTraversal(f);
    }
    if (right_) {
      right_->PostorderTraversal(f);
    }
    f(value_);
  }
private:
  void rotateRight() {
    auto lr = std::move(left_->right_);
    auto r = std::move(right_);
    
    right_ = std::shared_ptr<AVLTree>(new AVLTree(std::move(value_), shared_from_this()));
    right_->height_ = height_ - 1;
    left_->height_ -= 1;
    right_->right_ = std::move(r);
    right_->left_ = std::move(lr);

    value_ = std::move(left_->value_);
    left_ = std::move(left_->left_);
    if (left_) left_->parent_ = shared_from_this();
  }

  void rotateLeft() {
    auto rl = std::move(right_->left_);
    auto l = std::move(left_);

    left_ = std::shared_ptr<AVLTree>(new AVLTree(std::move(value_), shared_from_this()));
    left_->height_ = height_ - 1;
    right_->height_ -= 1;
    left_->left_ = std::move(l);
    left_->right_ = std::move(rl);

    value_ = std::move(right_->value_);
    right_ = std::move(right_->right_);
    
    if (right_) right_->parent_ = shared_from_this();
  }

};


const char TERM = '*';


class Trie {
  using Trie_ptr = std::unique_ptr<Trie>;
  std::map<char, Trie_ptr> suffixes_;

public:
  Trie()
    : suffixes_() {

  }

  void insert(const std::string& str) {
    if (str.empty()) {
      return;
    }
    auto it = suffixes_.insert({ str[0], std::make_unique<Trie>() }).first;
    for (auto ch = str.begin() + 1; ch != str.end(); ++ch) {
      it = it->second->suffixes_.insert({ *ch, std::make_unique<Trie>() }).first;
    }
    it->second->suffixes_.insert({ TERM, nullptr });
  }

  bool find(const std::string& str, bool substring = false) {
    if (str.empty()) {
      return true;
    }

    auto suffPtr = &suffixes_;
    for (auto ch = str.begin(); ch != str.end(); ++ch) {
      auto it = suffPtr->find(*ch);
      if (it == suffPtr->end()) {
        return false;
      }
      suffPtr = &it->second->suffixes_;
    }

    return (substring) ? true : (suffPtr->find(TERM) != suffPtr->end());
  }
};

} // namespace ds