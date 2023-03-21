#pragma once

#include "strong_typedef.h"

#include <engine/types.h>

#include <EASTL/vector.h>

#include <memory>

namespace Utils
{
  class StringMap
  {
    constexpr static size_t MIN_PAGE_BITS = 8;
    constexpr static size_t MAX_PAGE_BITS = 12;

    constexpr static size_t PAGE_ID_MASK = 0xffff;
    constexpr static size_t STRING_OFFSET_SHIFT = 16;

    struct Page
    {
      std::unique_ptr<char> buffer;
      size_t bufferSize;
      size_t bufferStart;

      Page(const size_t buffer_size);

      auto hasEnoughSpace(const size_t size) noexcept -> bool;
      auto writeBack(const char* src, size_t size) noexcept -> size_t;
    };

    public:
      STRONG_SCALAR_TYPEDEF(uint32_t, id_t);

      StringMap();

      auto storeString(string_view str) noexcept -> id_t;
      auto getString(const id_t id) const noexcept -> const char*;

    private:
      auto acquireFreePageId(const size_t required_space) noexcept -> size_t;

    private:
      eastl::vector<Page> m_Pages;
      size_t m_LastPageBits;
  };
}
