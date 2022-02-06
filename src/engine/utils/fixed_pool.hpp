#pragma once

#include "fixed_stack.hpp"
#include "bit_capacity.hpp"

namespace Utils
{
  template<class T, size_t N>
  class FixedPool
  {
    public:

    inline bool Add(T&& v, size_t& id)
    {
      if (m_Size < N)
      {
        const size_t i = AcquireFreePosition();
        m_Values[i] = std::forward<T>(v);

        id = i;
        return true;
      }
      else
        return false;
    }

    inline bool Remove(const size_t id)
    {
      ASSERT(id < N);

      m_Values[id].~T();
      const bool indexSaved = m_FreeIndices.Push(id);
      ASSERT(indexSaved);

      ASSERT(m_BitCapacity.IsSet(id));
      m_BitCapacity.Reset(id);

      return true;
    }

    private:
      inline size_t AcquireFreePosition()
      {
        size_t i = -1;
        if (m_FreeIndices.HasAny())
        {
          i = m_FreeIndices.GetLast();
          m_FreeIndices.Pop();
        }
        else
        {
          i = m_Size + 1;
        }

        m_Size += 1;
        m_BitCapacity.Set(i);

        return i;
      }

    private:
      T m_Values[N];
      size_t m_Size = 0;

      BitCapacity<N> m_BitCapacity;
      FixedStack<size_t, N> m_FreeIndices;
  };
}