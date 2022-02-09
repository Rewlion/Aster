#pragma once

namespace Utils
{
  template<class T, size_t N>
  class FixedStack
  {
    public:
      inline T* GetData()
      {
        return m_Values;
      }

      inline const T* GetData() const
      {
        return m_Values;
      }

      inline size_t GetSize() const
      {
        return m_Size;
      }

      inline size_t HasAny() const
      {
        return m_Size != 0;
      }

      inline T& GetLast()
      {
        return m_Values[m_Size-1];
      }

      inline const T& GetLast() const
      {
        return const_cast<FixedStack*>(this)->GetLast();
      }

      inline T& GetFirst()
      {
        return m_Values[0];
      }

      inline const T& GetFirst() const
      {
        return const_cast<FixedStack*>(this)->GetFirst();
      }

      bool Push(T&& v)
      {
        if (m_Size < N)
        {
          m_Values[m_Size++] = std::forward<T>(v);
          return true;
        }
        else
          return false;
      }

      bool Push(const T& v)
      {
        if (m_Size < N)
        {
          m_Values[m_Size++] = v;
          return true;
        }
        else
          return false;
      }

      bool Pop()
      {
        if (m_Size > 0)
        {
          GetLast().~T();
          --m_Size;
          return true;
        }
        else
          return false;
      }

    class ConstIterator
    {
      public:
        ConstIterator(const T* values, size_t pos)
          : m_Values(values)
          , m_Pos(pos)
        {
        }

        const T& operator*() const
        {
          return m_Values[m_Pos];
        }

        const T& operator++()
        {
          return m_Values[++m_Pos];
        }

        const T& operator++(int)
        {
          return m_Values[m_Pos++];
        }

        bool operator==(const ConstIterator& it) const
        {
          return m_Values == it.m_Values && m_Pos == it.m_Pos;
        }

        bool operator!=(const ConstIterator& it) const
        {
          return m_Values != it.m_Values || m_Pos != it.m_Pos;
        }

      private:
        const T* m_Values;
        size_t m_Pos;
    };

    ConstIterator begin() const
    {
      return ConstIterator(m_Values, 0);
    }

    ConstIterator end() const
    {
      return ConstIterator(m_Values, m_Size);
    }

    private:
      T m_Values[N];
      size_t m_Size = 0;
  };
}