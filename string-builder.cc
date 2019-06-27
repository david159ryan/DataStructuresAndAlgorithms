#include "string-builder.h"

namespace ds {

static auto length_accumulator_func = [](const size_t & a, const std::string & b) -> size_t {
  return a + b.length();
};

StringBuilder::StringBuilder()
    : length_(0) {

}

StringBuilder::StringBuilder(std::initializer_list<std::string> const& l)
    : ArrayList<std::string>(l),
      length_(std::accumulate(l.begin(), l.end(), (size_t)0,
          length_accumulator_func)) {

}

StringBuilder::~StringBuilder() {

}

std::string StringBuilder::Build() {
  std::string retval;
  retval.reserve(length_);

  std::for_each(this->begin(), this->end(), [&retval](std::string & val) {
    retval.append(val);
  });
  return retval;
}

size_t StringBuilder::Length() {
  return length_;
}

void StringBuilder::Append(std::string val)
{
  if (val.empty()) return;

  length_ += val.length();
  ArrayList::Append(std::forward<std::string>(val));
}

std::string StringBuilder::Pop() {
  return this->Remove(this->Size() - 1);
}

void StringBuilder::Add(size_t index, const std::string& val) {
  length_ += val.length();
  ArrayList::Add(index, val);
}

void StringBuilder::Add(size_t index, std::string&& val) {
  length_ += val.length();
  ArrayList::Add(index, val);
}

void StringBuilder::Add(size_t index, std::initializer_list<std::string> const& l) {
  length_ = std::accumulate(l.begin(), l.end() , length_, length_accumulator_func);
  ArrayList::Add(index, l.begin(), l.end());
}

void StringBuilder::Set(size_t index, std::string val) {
  length_ += ArrayList::Get(index).length() -  val.length();
  ArrayList::Set(index, std::forward<std::string>(val));
}

std::string StringBuilder::Remove(size_t index) {
  length_ -= ArrayList::Get(index).length();
  return ArrayList::Remove(index);
}

} //namespace ds
