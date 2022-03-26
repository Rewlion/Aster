#pragma once

#include <type_traits>

namespace Utils
{
  template<class... T>
  struct TypeUnpack;

  template<class First>
  struct TypeUnpack<First>
  {
  };

  template<class First, class... Rest>
  struct TypeUnpack<First, Rest...>
  {
  };

  template<class... T>
  struct TypePack
  {
  };

  template<size_t id, class Target, class Types>
  struct TypeIdx;

  template<size_t id, class Target, class First, class... Rest>
  struct TypeIdx<id, Target, TypeUnpack<First, Rest...>>
  {
    constexpr static size_t get()
    {
      if constexpr ( std::is_same<Target, First>::value )
        return id;
      else
        return TypeIdx<id+1, Target, TypeUnpack<Rest...>>::get();
    }
  };

  template<size_t id, class Target, class Last>
  struct TypeIdx<id, Target, TypeUnpack<Last>>
  {
    constexpr static size_t get()
    {
      if constexpr ( std::is_same<Target, Last>::value )
        return id;
      else
        static_assert(!"Type is not inside Types array");
    }
  };

  template<class Target, class... Types>
  constexpr size_t get_type_Id()
  {
    return TypeIdx<0, Target, TypeUnpack<Types...>>::get();
  }

  template<class... T>
  class TypesArray
  {
  };

  template<class First, class... Rest>
  class TypesArray<First, Rest...>
  {
    public:
      TypesArray()
      {
      }

      TypesArray(const First& first, const Rest&... rest)
        : m_First(first)
        , m_Rest(rest...)
        {
        }

      First& getFirst()
      {
        return m_First;
      }

      TypesArray<Rest...>& getRest()
      {
        return m_Rest;
      }

      template<size_t id>
      decltype(auto) get()
      {
        return getType<id>(*this);
      }

      template<class RequiredType>
      decltype(auto) get()
      {
        return doGet<RequiredType, First, Rest...>();
      }

    private:
      template<class RequiredType, class... Types>
      decltype(auto) doGet()
      {
        return getType<RequiredType, Types...>(*this);
      }

      template<size_t id, class T>
      struct TypeGetter;

      template<class First, class... Rest>
      struct TypeGetter<0, TypesArray<First, Rest...>>
      {
        static decltype(auto) get(TypesArray<First, Rest...>& data)
        {
          return data.getFirst();
        }
      };

      template<size_t id, class First, class... Rest>
      struct TypeGetter<id, TypesArray<First, Rest...>>
      {
        static decltype(auto) get(TypesArray<First, Rest...>& data)
        {
          return TypeGetter<id-1,  TypesArray<Rest...>>::get(data.getRest());
        }
      };

      template<class... T>
      decltype(auto) getType(TypesArray<T...>& data)
      {
        static_assert(!"unknown type");
      }

      template<size_t id, class First, class... Rest>
      decltype(auto) getType(TypesArray<First, Rest...>& data)
      {
        return TypeGetter<id, TypesArray<First, Rest...>>::get(data);
      }

      template<class RequiredType, class... Rest>
      decltype(auto) getType(TypesArray<Rest...>& data)
      {
        constexpr size_t id = TypeIdx<0, RequiredType, TypeUnpack<Rest...>>::get();
        return getType<id>(data);
      }

    private:
      First m_First;
      TypesArray<Rest...> m_Rest;
  };
}
