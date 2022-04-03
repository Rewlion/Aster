#pragma once

namespace Sys
{
  #ifdef PLATFORM_WIN64
    constexpr size_t alignment = 4;
  #else
    constexpr size_t alignment = 4;
  #endif

  inline static void align(size_t& v)
  {
    const size_t r = v % Sys::alignment;
    if (r > 0)
      v = v + (Sys::alignment - r);
  }

  inline static void align(size_t& v, const size_t alignment)
  {
    const size_t r = v % alignment;
    if (r > 0)
      v = v + (alignment - r);
  }
}
