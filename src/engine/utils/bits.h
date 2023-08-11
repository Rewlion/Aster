#pragma once

#include <engine/types.h>

namespace Utils
{
  template<class T>
  void set_bit(T& in, const uint8_t bit)
  {
    in |= (bit);
  }

  template<class T>
  void clear_bit(T& in, const uint8_t bit)
  {
    in &= ~(bit);
  }

  template<class T>
  void toggle_bit(T& in, const uint8_t bit)
  {
    in = (T)(in ^ (1 << bit));
  }

  inline
  auto is_pow_of_2(const uint n) -> bool
  {
    return (n & (n - 1)) == 0;
  }
}
