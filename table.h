#pragma once
#include <string>
#include <optional>
#include "list.h"

namespace ds {

template <class K, class V>
class HashTable {
public:
  using key_type = K;
  using value_type = V;
  using table_entry = std::pair<key_type, value_type>;
private:
  static inline size_t NUM_BUCKETS = 255;

  SLList<table_entry> buckets_[255];
  size_t count_;

public:
  HashTable() 
    : count_(0) {

  }

  HashTable(std::initializer_list<table_entry> init) 
      : count_(0){
    std::for_each(init.begin(), init.end(), [this](auto&& val) {
      this->Insert(val);
    });
  }

  size_t Size() const {
    return count_;
  }

  bool isEmpty() const {
    return count_ == 0;
  }

  template <typename table_entry>
  bool Insert(table_entry&& entry) {
    auto found = Find(entry.first);
    auto newInserted = !found.has_value();

    if (!newInserted) {
      (**found) = std::move(entry.second);
    }
    else {
      ++count_;
      buckets_[hash(entry)].Append(std::forward<table_entry>(entry));
    }
    return newInserted;
  }

  std::optional<value_type * const> Find(key_type key) const {
    size_t index = hash(key);
    auto it = std::find_if(buckets_[index].begin(), buckets_[index].end(), [&key](const table_entry& val) {
      return val.first == key;
    });
    
    return (it == buckets_[index].end()) ? std::nullopt : std::optional<value_type*>{&(*it).second};
  }

  std::optional<value_type> Remove(key_type key) {
    size_t index = hash(key);
    auto it = std::find_if(buckets_[index].begin(), buckets_[index].end(), [&key](const table_entry& val) {
      return val.first == key;
    });
    auto val = buckets_[index].Remove(std::distance(it, buckets_[index].begin()));

    std::optional<value_type> retval = std::nullopt;

    if (it != buckets_[index].end()) {
      retval = std::optional<value_type>{ std::move(val.second) };
      --count_;
    }
    return retval;
  }

private:
  size_t hash(const key_type key) const {
    return std::hash<key_type>{}(key) % NUM_BUCKETS;
  }

  size_t hash(const table_entry& entry) const {
    return hash(entry.first);
  }
};

} // namespace ds