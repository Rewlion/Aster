#pragma once

#include <string>
#include <functional>
#include <vector>
#include <array>

namespace Meta
{
  struct Type
  {
    std::string name;
    size_t size;
  };

  struct Attribute
  {
    const Type* type = nullptr;
    std::string name;
    size_t offset = 0;
  };
  
  struct Class: public Type
  {
    std::string headerFile;
    Attribute* attributesBegin;
    size_t attributesNumber;
  };

  

  template<size_t N>
  constexpr const size_t GetValidArraySize()
  {
    // array must have at least 1 element | cpp20 [dcl.array]
    if constexpr (N == 0)
      return 1;
    else
      return N;
  }

  template<size_t AttributesNumber>
  struct ClassStorage
  {
    ClassStorage(std::function<void(ClassStorage&)> constructor)
    {
      constructor(*this);
    }

    Attribute attributes[GetValidArraySize<AttributesNumber>()];
  };

  template<class T> struct TypeTag{};

  class TypeAccessor
  {
  public:
    template<class T> static
    const Type* GetTypeImpl(TypeTag<T>);

    template<class T> static
    const Type* GetType()
    {
      return GetTypeImpl(TypeTag<T>{});
    }

    template<class T> static
    const Class* GetClassImpl(TypeTag<T>);

    template<class T> static
    const Class* GetClass()
    {
      return GetClassImpl(TypeTag<T>{});
    }
  };
}

#include "fundamental_types.h"
