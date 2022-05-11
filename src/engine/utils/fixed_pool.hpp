#pragma once

#include "fixed_stack.hpp"
#include "bit_capacity.hpp"

namespace Utils
{
  template<class T, size_t N>
  class FixedPool
  {
    public:

    inline bool add(T&& v, size_t& id)
    {
      if (m_Size < N)
      {
        const size_t i = acquireFreePosition();
        m_Values[i] = std::forward<T>(v);

        id = i;
        return true;
      }
      else
        return false;
    }

    inline bool remove(const size_t id)
    {
      ASSERT(id < N);

      m_Values[id].~T();
      std::memset(&m_Values[id], 0, sizeof(T));

      const bool indexSaved = m_FreeIndices.push(id);
      ASSERT(indexSaved);

      ASSERT(m_BitCapacity.isSet(id));
      m_BitCapacity.reset(id);

      return true;
    }

    inline bool contains(const size_t id)
    {
       return m_BitCapacity.isSet(id);
    }

    inline T& get(const size_t id)
    {
      return m_Values[id];
    }

    inline const T& get(const size_t id) const
    {
      return const_cast<FixedPool*>(this)->get(id);
    }

    private:
      inline size_t acquireFreePosition()
      {
        size_t i = -1;
        if (m_FreeIndices.hasAny())
        {
          i = m_FreeIndices.getLast();
          m_FreeIndices.pop();
        }
        else
        {
          i = m_Size++;
        }

        m_BitCapacity.set(i);

        return i;
      }

    private:
      T m_Values[N];
      size_t m_Size = 0;

      BitCapacity<N> m_BitCapacity;
      FixedStack<size_t, N> m_FreeIndices;
  };
}