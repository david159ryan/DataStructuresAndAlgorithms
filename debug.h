#pragma once

#include <cassert>
#include <iostream>

namespace ds {
  
inline bool check_bounds(size_t index, size_t size) {
  if (index >= size)
  {
    std::cerr << "Index '" << index << "' is out of bounds: Size is '" << size << "'" << std::endl;
    std::cerr << "Aborting..." << std::endl;

    abort();
  }

  return true;
}

} // namespace ds