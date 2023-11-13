#pragma once

#include <iterator>
#include <type_traits>

namespace Utils
{
    template<class T>
    class Iterator
    {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        constexpr
        Iterator(T* values, size_t pos)
          : m_Values(values)
          , m_Pos(pos)
        {
        }

        constexpr
        reference operator*() const
        {
          return m_Values[m_Pos];
        }

        constexpr
        Iterator operator++()
        {
          return Iterator{m_Values, ++m_Pos};
        }

        constexpr
        bool operator==(const Iterator& it) const
        {
          return m_Values == it.m_Values && m_Pos == it.m_Pos;
        }

        constexpr
        bool operator!=(const Iterator& it) const
        {
          return m_Values != it.m_Values || m_Pos != it.m_Pos;
        }

      private:
        pointer m_Values;
        size_t m_Pos;
  };

  template<class T, size_t N>
  class FixedStack
  {
    public:
      using value_type      = T;
      using size_type       = size_t;
      using pointer         = T*;
      using const_pointer   = const T*;
      using reference       = T&;
      using const_reference = const T&;

      using iterator        = Iterator<T>;
      using const_iterator  = Iterator<const T>;

    public:

      template<class... Args>
      constexpr
      FixedStack(Args... args)
      {
        static_assert(sizeof...(args) <= N);
        (push(args), ...);
      }

      constexpr
      FixedStack(FixedStack&& rvl) = default;

      constexpr
      FixedStack(const FixedStack& rvl) = default;

      constexpr
      FixedStack() = default;

      ~FixedStack() = default;
      ~FixedStack() requires (!std::is_trivially_copyable_v<T>)
      {
        clear();
      }

      constexpr
      pointer getData()
      {
        return m_Values;
      }

      constexpr
      const_pointer getData() const
      {
        return m_Values;
      }

      constexpr
      size_type count() const
      {
        return m_Size;
      }

      constexpr
      bool hasAny() const
      {
        return m_Size != 0;
      }

      constexpr
      reference getLast()
      {
        return m_Values[m_Size-1];
      }

      constexpr
      const_reference getLast() const
      {
        return const_cast<FixedStack*>(this)->getLast();
      }

      constexpr
      reference getFirst()
      {
        return m_Values[0];
      }

      constexpr
      const_reference getFirst() const
      {
        return const_cast<FixedStack*>(this)->getFirst();
      }

      constexpr
      bool empty() const
      {
        return m_Size > 0;
      }

      constexpr
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

      constexpr
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

      constexpr
      bool pop()
      {
        if (m_Size > 0)
        {
          getLast().~T();
          std::memset(&getLast(), 0, sizeof(T));

          --m_Size;
          return true;
        }
        else
          return false;
      }

      constexpr
      reference get(const size_type i)
      {
        return m_Values[i];
      }

      constexpr
      const_reference get(const size_type i) const
      {
        return const_cast<FixedStack*>(this)->get(i);
      }

      constexpr
      FixedStack& operator=(const FixedStack& rvl) requires (std::is_trivially_copyable_v<T>) = default;
      constexpr
      FixedStack& operator=(const FixedStack& rvl) requires (!std::is_trivially_copyable_v<T>)
      {
        clear();
        for(const auto& v: rvl)
          push(v);

        return *this;
      }

      constexpr
      FixedStack& operator=(FixedStack&& rvl) requires (std::is_trivially_copyable_v<T>) = default;
      constexpr
      FixedStack& operator=(FixedStack&& rvl) requires (!std::is_trivially_copyable_v<T>)
      {
        clear();
        for(auto& v: rvl)
          push(std::move(v));

        rvl.clear();

        return *this;
      }

      constexpr
      bool operator==(const FixedStack& rvl) const
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

      constexpr
      void clear()
      {
        for (size_type i = 0, count = m_Size; i < count; ++i)
          pop();
      }

      constexpr
      decltype(auto) begin() const
      {
        return const_iterator(m_Values, 0);
      }

      constexpr
      decltype(auto) end() const
      {
        return const_iterator(m_Values, m_Size);
      }

      constexpr
      decltype(auto) begin()
      {
        return iterator(m_Values, 0);
      }

      decltype(auto) end()
      {
        return iterator(m_Values, m_Size);
      }

      constexpr
      auto size() const -> size_type
      {
        return N;
      }

    private:
      value_type m_Values[N];
      size_type m_Size = 0;
  };
}