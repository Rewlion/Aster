#include "string_map.h"

#define XXH_INLINE_ALL
#include <xxhash/xxhash.h>

namespace Utils
{
  StringMap::Page::Page(const size_t buffer_size)
  {
    buffer.reset(new char[buffer_size]);
    bufferSize = buffer_size;
    freeSpaceBegin = 0;
  }

  auto StringMap::Page::hasEnoughSpace(const size_t size) noexcept -> bool
  {
    return (freeSpaceBegin + size + 1) < bufferSize;
  }

  auto StringMap::Page::writeBack(const char* src, size_t size) noexcept -> size_t
  {
    const size_t start = freeSpaceBegin;
    char* buf = buffer.get() + start;

    std::memcpy(buf, src, size);
    buf[size] = '\0';
    freeSpaceBegin = freeSpaceBegin + size + 1;

    return start;
  }

  StringMap::StringMap()
    : m_LastPageBits(MIN_PAGE_BITS)
  {
  }

  auto StringMap::storeString(const char* str) noexcept -> name_id_t
  {
    return storeString(str, std::strlen(str));
  }

  auto StringMap::storeString(const char* str, const size_t size) noexcept -> name_id_t
  {
    const auto hash = hashString(str, size);
    const auto it = m_NameHashToId.find(hash);
    if (it != m_NameHashToId.end())
      return it->second;

    const size_t pageId = acquireFreePageId(size);
    const size_t offset = m_Pages[pageId].writeBack(str, size);

    const uint32_t allocId = offset << STRING_OFFSET_SHIFT | pageId;
    const name_id_t newId{(int)m_AllocIds.size()};
    
    m_AllocIds.emplace_back(allocId);
    m_NameHashToId.insert({hash, newId});

    return newId;
  }

  auto StringMap::getString(const name_id_t id) const noexcept -> const char*
  {
    const alloc_id_t allocId = m_AllocIds[id];

    const size_t pageId = allocId & PAGE_ID_MASK;
    const size_t offset = allocId >> STRING_OFFSET_SHIFT;

    return m_Pages[pageId].buffer.get() + offset;
  }

  auto StringMap::getStringId(const char* str) const noexcept -> name_id_t
  {
    const auto hash = hashString(str, std::strlen(str));
    const auto it = m_NameHashToId.find(hash);
    if (it != m_NameHashToId.end())
      return it->second;
    
    return INVALID_NAME_ID;
  }


  auto StringMap::hashString(const char* str, const size_t size) const noexcept -> uint32_t
  {
    const uint32_t seed = 0;
    const uint32_t hash = XXH32(str, size, seed);
    return hash;
  }

  auto StringMap::acquireFreePageId(const size_t required_space) noexcept -> size_t
  {
    const size_t pagesCount = m_Pages.size();

    for (size_t i = 0; i < pagesCount; ++i)
      if (m_Pages[i].hasEnoughSpace(required_space))
        return i;

    m_LastPageBits = std::min(MAX_PAGE_BITS, m_LastPageBits+1);
    const size_t id = m_Pages.size();
    m_Pages.emplace_back(1 << m_LastPageBits);

    return id;
  }

  void StringMap::clear()
  {
    m_Pages.clear();
    m_NameHashToId.clear();
    m_AllocIds.clear();
    m_LastPageBits = MIN_PAGE_BITS;
  }
}
