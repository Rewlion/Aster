#pragma once

#include "forward_decl.h"

namespace qjs
{
  class ArrayView
  {
    public:
      ArrayView(const Value&);
      ArrayView(ArrayView&& rvl);
      ArrayView(const ArrayView& rvl) = delete;

      inline size_t length() const
      {
        return m_Length;
      }

      Value operator[](const uint32_t i) const;

    private:
      size_t m_Length;
      const Value& m_Value;
  };
}
