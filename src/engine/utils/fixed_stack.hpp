#pragma once

namespace Utils
{
    template<class T>
    class Iterator
    {
      public:
        Iterator(T* values, size_t pos)
          : m_Values(values)
          , m_Pos(pos)
        {
        }

        T& operator*() const
        {
          return m_Values[m_Pos];
        }

        Iterator operator++()
        {
          return Iterator{m_Values, ++m_Pos};
        }

        bool operator==(const Iterator& it) const
        {
          return m_Values == it.m_Values && m_Pos == it.m_Pos;
        }

        bool operator!=(const Iterator& it) const
        {
          return m_Values != it.m_Values || m_Pos != it.m_Pos;
        }

      private:
        T* m_Values;
        size_t m_Pos;
  };

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

      FixedStack(FixedStack<T,N>&& rvl)
      {
        *(this) = std::move(rvl);
      }

      FixedStack(const FixedStack<T,N>& rvl)
      {
        *(this) = rvl;
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
          m_Values[m_Size++] = std::move(v);
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

      inline const FixedStack<T,N>& operator=(FixedStack<T,N>&& rvl)
      {
        clear();
        for(auto& v: rvl)
          push(std::move(v));

        rvl.clear();

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

    decltype(auto) begin() const
    {
      return Iterator<const T>(m_Values, 0);
    }

    decltype(auto) end() const
    {
      return Iterator<const T>(m_Values, m_Size);
    }

    decltype(auto) begin()
    {
      return Iterator<T>(m_Values, 0);
    }

    decltype(auto) end()
    {
      return Iterator<T>(m_Values, m_Size);
    }

    private:
      T m_Values[N];
      size_t m_Size = 0;
  };
}