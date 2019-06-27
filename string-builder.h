#pragma once

#include <string>

#include "list.h"

namespace ds {

class StringBuilder : public ArrayList<std::string> {
public:
  StringBuilder();
  StringBuilder(std::initializer_list<std::string> const& l);
  ~StringBuilder();

  std::string Build();
  size_t Length();

  void Append(std::string val);
  std::string Pop();
  void Add(size_t index, const std::string& val);
  void Add(size_t index, std::string&& val);
  void Add(size_t index, std::initializer_list<std::string> const& l);
  void Set(size_t index, std::string val);
  std::string Remove(size_t index);

private:
  size_t length_;

};

} //namespace ds
