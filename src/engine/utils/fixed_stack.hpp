#pragma once

namespace Utils
{
  template<class T, size_t N>
  class FixedStack
  {
    public:
      template<class... Args>
      FixedStack(Args... args)
      {
        static_assert(sizeof...(args) <= N);
        (push(args), ...);
      }

      FixedStack()
      {
      }

      inline T* getData()
      {
        return m_Values;
      }

      inline const T* getData() const
      {
        return m_Values;
      }

      inline size_t getSize() const
      {
        return m_Size;
      }

      inline size_t hasAny() const
      {
        return m_Size != 0;
      }

      inline T& getLast()
      {
        return m_Values[m_Size-1];
      }

      inline const T& getLast() const
      {
        return const_cast<FixedStack*>(this)->getLast();
      }

      inline T& getFirst()
      {
        return m_Values[0];
      }

      inline const T& getFirst() const
      {
        return const_cast<FixedStack*>(this)->getFirst();
      }

      bool push(T&& v)
      {
        if (m_Size < N)
        {
          m_Values[m_Size++] = std::forward<T>(v);
          return true;
        }
        else
          return false;
      }

      bool push(const T& v)
      {
        if (m_Size < N)
        {
          m_Values[m_Size++] = v;
          return true;
        }
        else
          return false;
      }

      bool pop()
      {
        if (m_Size > 0)
        {
          getLast().~T();
          --m_Size;
          return true;
        }
        else
          return false;
      }

      inline T& get(const size_t i)
      {
        return m_Values[i];
      }

      inline const T& get(const size_t i) const
      {
        return const_cast<FixedStack*>(this)->get(i);
      }

      inline const FixedStack<T,N>& operator=(const FixedStack<T,N>& rvl)
      {
        clear();
        for(const auto& v: rvl)
          push(v);

        return *this;
      }

      inline bool operator==(const FixedStack<T,N>& rvl)
      {
        if (rvl.m_Size == m_Size)
        {
          if (m_Size == 0)
            return true;

          for (size_t i = 0; i < m_Size; ++i)
          {
            if ( get(i) != rvl.get(i) )
              break;

            return true;
          }
        }
        return false;
      }

      inline void clear()
      {
        for (size_t i = 0; i < m_Size; ++i)
          pop();
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