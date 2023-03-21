#include "string_map.h"

namespace Utils
{
  StringMap::Page::Page(const size_t buffer_size)
  {
    buffer.reset(new char[buffer_size]);
    bufferSize = buffer_size;
    bufferStart = 0;
  }

  auto StringMap::Page::hasEnoughSpace(const size_t size) noexcept -> bool
  {
    return (bufferStart + size + 1) < bufferSize;
  }

  auto StringMap::Page::writeBack(const char* src, size_t size) noexcept -> size_t
  {
    const size_t start = bufferStart;
    std::memcpy(buffer.get(), src, size);
    buffer.get()[bufferStart + size] = '\0';
    bufferStart = bufferStart + size + 1;

    return start;
  }

  StringMap::StringMap()
    : m_LastPageBits(MIN_PAGE_BITS)
  {
  }

  auto StringMap::storeString(string_view str) noexcept -> StringMap::id_t
  {
    const size_t pageId = acquireFreePageId(str.length());
    const size_t offset = m_Pages[pageId].writeBack(str.data(), str.length());

    return id_t{ offset << STRING_OFFSET_SHIFT | pageId };
  }

  auto StringMap::getString(const id_t id) const noexcept -> const char*
  {
    const size_t pageId = id & PAGE_ID_MASK;
    const size_t offset = id >> STRING_OFFSET_SHIFT;

    return m_Pages[pageId].buffer.get() + offset;
  }

  auto StringMap::acquireFreePageId(const size_t required_space) noexcept -> size_t
  {
    const size_t pagesCount = m_Pages.size();

    for (size_t i = 0; i < pagesCount; ++i)
      if (m_Pages[i].hasEnoughSpace(required_space))
        return i;

    m_LastPageBits = std::min(MAX_PAGE_BITS, m_LastPageBits+1);
    const size_t id = m_Pages.size();
    m_Pages.emplace_back(m_LastPageBits << 1);

    return id;
  }
}
