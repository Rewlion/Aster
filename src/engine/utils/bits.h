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
}
