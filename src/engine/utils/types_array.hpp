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
    constexpr static size_t Get()
    {
      if constexpr ( std::is_same<Target, First>::value )
        return id;
      else
        return TypeIdx<id+1, Target, TypeUnpack<Rest...>>::Get();
    }
  };

  template<size_t id, class Target, class Last>
  struct TypeIdx<id, Target, TypeUnpack<Last>>
  {
    constexpr static size_t Get()
    {
      if constexpr ( std::is_same<Target, Last>::value )
        return id;
      else
        static_assert(!"Type is not inside Types array");
    }
  };

  template<class Target, class... Types>
  constexpr size_t GetTypeId()
  {
    return TypeIdx<0, Target, TypeUnpack<Types...>>::Get();
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

      First& GetFirst()
      {
        return m_First;
      }

      TypesArray<Rest...>& GetRest()
      {
        return m_Rest;
      }

      template<size_t id>
      decltype(auto) Get()
      {
        return GetType<id>(*this);
      }

      template<class RequiredType>
      decltype(auto) Get()
      {
        return DoGet<RequiredType, First, Rest...>();
      }

    private:
      template<class RequiredType, class... Types>
      decltype(auto) DoGet()
      {
        return GetType<RequiredType, Types...>(*this);
      }

      template<size_t id, class T>
      struct TypeGetter;

      template<class First, class... Rest>
      struct TypeGetter<0, TypesArray<First, Rest...>>
      {
        static decltype(auto) Get(TypesArray<First, Rest...>& data)
        {
          return data.GetFirst();
        }
      };

      template<size_t id, class First, class... Rest>
      struct TypeGetter<id, TypesArray<First, Rest...>>
      {
        static decltype(auto) Get(TypesArray<First, Rest...>& data)
        {
          return TypeGetter<id-1,  TypesArray<Rest...>>::Get(data.GetRest());
        }
      };

      template<class... T>
      decltype(auto) GetType(TypesArray<T...>& data)
      {
        static_assert(!"unknown type");
      }

      template<size_t id, class First, class... Rest>
      decltype(auto) GetType(TypesArray<First, Rest...>& data)
      {
        return TypeGetter<id, TypesArray<First, Rest...>>::Get(data);
      }

      template<class RequiredType, class... Rest>
      decltype(auto) GetType(TypesArray<Rest...>& data)
      {
        constexpr size_t id = TypeIdx<0, RequiredType, TypeUnpack<Rest...>>::Get();
        return GetType<id>(data);
      }

    private:
      First m_First;
      TypesArray<Rest...> m_Rest;
  };
}
