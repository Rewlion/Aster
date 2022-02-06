#pragma once

namespace Utils
{
  template<class T, size_t N>
  class FixedStack
  {
    public:
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
          m_Values[m_Size++] = std::forward(v);
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

    private:
      T m_Values[N];
      size_t m_Size = 0;
  };
}