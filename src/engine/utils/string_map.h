#pragma once

#include "strong_typedef.h"

#include <engine/types.h>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

#include <memory>

STRONG_SCALAR_TYPEDEF(int, name_id_t);
constexpr name_id_t INVALID_NAME_ID{-1};

namespace Utils
{
  class StringMap
  {
    STRONG_SCALAR_TYPEDEF(uint32_t, alloc_id_t);

    constexpr static size_t MIN_PAGE_BITS = 8;
    constexpr static size_t MAX_PAGE_BITS = 12;

    constexpr static size_t PAGE_ID_MASK = 0xffff;
    constexpr static size_t STRING_OFFSET_SHIFT = 16;

    struct Page
    {
      std::unique_ptr<char> buffer;
      size_t bufferSize;
      size_t freeSpaceBegin;

      Page(const size_t buffer_size);

      auto hasEnoughSpace(const size_t size) noexcept -> bool;
      auto writeBack(const char* src, size_t size) noexcept -> size_t;
    };

    public:
      StringMap();

      auto storeString(const char* str) noexcept -> name_id_t;
      auto storeString(const char* str, const size_t size) noexcept -> name_id_t;
      auto getString(const name_id_t id) const noexcept -> const char*;
      auto getStringId(const char* str) const noexcept -> name_id_t;

      void clear();

    private:
      auto hashString(const char* str, const size_t size) const noexcept -> uint32_t;
      auto acquireFreePageId(const size_t required_space) noexcept -> size_t;

    private:
      eastl::vector<Page> m_Pages;
      eastl::vector_map<uint32_t, name_id_t> m_NameHashToId;
      eastl::vector<alloc_id_t> m_AllocIds;
      size_t m_LastPageBits;
  };
}
