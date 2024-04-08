#pragma once

#include "core_types.h"
#include "type_meta.h"
#include "hash.h"

#include <function2/function2.hpp>

namespace ed
{
  class TypeConstructor;
}

namespace ecs
{
  struct TemplateTypeWrapper
  {
    string typeName;
  };

  class TemplateComponent
  {
    friend class Registry;
    friend class TemplateComponentsMap;
    friend class EntityComponents;

    public:
      TemplateComponent();
      TemplateComponent(TemplateComponent&&);
      ~TemplateComponent();

      auto operator=(TemplateComponent&&) -> TemplateComponent&;

      void operator=(const ed::TypeConstructor&);

      template<class T>
      void operator=(T&& v) &;

      auto getMeta() const -> const TypeMeta*;

      auto getNameHash() const -> name_hash_t;

    private:
      TemplateComponent(const name_hash_t hash);

      template<class T>
      static
      auto isBoxedType() -> bool;

      static
      auto isBoxedType(const size_t size) -> bool;

      auto getData() const -> const void*;
      auto getData() -> void*;

      void initTypeIdFromName(const string&);

    private:
      enum
      {
        COMPONENT_TYPE_WRAPPER = 1,
      };

      union
      {
        void* ptr;
        uint64_t rawValue;
      } as;

      fu2::function_base<true, false, fu2::capacity_none,
        false, true, void(void*, const TypeManager*) const> destructor;
      component_type_id_t m_TypeId;
      name_hash_t m_NameHash;
      uint8_t m_Flags = 0;
  };

  template<class T>
  void TemplateComponent::operator=(T&& v) &
  {
    using Type = typename std::remove_cvref<T>::type;
    if constexpr (std::is_same<TemplateTypeWrapper, Type>::value)
    {
      initTypeIdFromName(v.typeName);
      m_Flags |= COMPONENT_TYPE_WRAPPER;
    }
    else
    {
      if (isBoxedType<T>())
      {
        using PtrType = typename std::remove_cvref<T>::type;
        as.ptr = new PtrType;
        *(PtrType*)as.ptr = std::move(v);

        destructor = [](void* data, const TypeManager*)
        {
          PtrType* typedData = reinterpret_cast<PtrType*>(data);
          delete typedData;
        };
      }
      else
      {
        new (&as.rawValue) Type{std::forward<T>(v)};
        using PtrType = typename std::remove_cvref<T>::type;
        destructor = [](void* data, const TypeManager*)
        {
          PtrType* typedData = reinterpret_cast<PtrType*>(data);
          typedData->~PtrType();
        };
      }
      m_TypeId = CompileTypeId::from<T>();
    }
  }

  template<class T>
  auto TemplateComponent::isBoxedType() -> bool
  {
    return isBoxedType(sizeof(T));
  }

  class TemplateComponentsMap
  {
    public:
      auto operator[] (const string_view name) -> TemplateComponent&;

      auto getComponent(const name_hash_t name) const -> const TemplateComponent&;

      auto getMap() const -> const eastl::vector_map<name_hash_t, TemplateComponent>&;

      auto getComponentName(const name_hash_t name) const -> string_view;

      auto empty() const -> bool { return m_Names.empty(); }

    private:
      eastl::vector_map<name_hash_t, TemplateComponent> m_Components;
      eastl::vector_map<name_hash_t, string> m_Names;
  };
}
