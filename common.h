#pragma once

namespace ds {

constexpr size_t abs_diff(size_t a, size_t b) {
  return (a < b) ? b - a : a - b;
}

} // namespace ds