#pragma once

namespace sys
{
  #ifdef PLATFORM_WIN64
    constexpr size_t alignment = 4;
  #else
    constexpr size_t alignment = 4;
  #endif

  inline static void align(size_t& v)
  {
    const size_t r = v % sys::alignment;
    if (r > 0)
      v = v + (sys::alignment - r);
  }
}
