#pragma once

#include "core_types.h"

#include <engine/assert.h>

#include <EASTL/vector_map.h>

namespace ecs
{
  class CompileTypeId
  {
    public:
      template<class T>
      static
      auto from() -> component_type_id_t;

    private:
      template<class T>
      static
      auto fromInternal() -> component_type_id_t;

    private:
      static component_type_id_t m_LastId;
  };

  class TypeManager
  {
    public:
      virtual
      void constructor(void* component) = 0;

      virtual
      void constructor(void* __restrict__ component, const void* __restrict__ init_comp) = 0;
      
      virtual
      void moveConstructor(void* __restrict__ component, void* __restrict__ init_comp) = 0;

      virtual
      void destructor(void* component) = 0;

      // virtual
      // void copy(void* __restrict__ to, const void* __restrict__ from) {}

      // virtual
      // void move(void* __restrict__ to, void* __restrict__ from) {}
  };

  template<class T>
  class TrivialTypeManager : public TypeManager
  {
    public:
      void constructor(void* component) override
      {
        *reinterpret_cast<T*>(component) = T{};
      }

      void constructor(void* __restrict__ component, const void* __restrict__ init_comp) override
      {
        std::memcpy(component, init_comp, sizeof(T));
      }

      void moveConstructor(void* __restrict__ component, void* __restrict__ init_comp) override
      {
         std::memcpy(component, init_comp, sizeof(T));
      }

      void destructor(void* component) override
      {
      }

      // void copy(void* __restrict__ to, const void* __restrict__ from) override
      // {
      //   std::memcpy(to, from, sizeof(T));
      // }

      // void move(void* __restrict__ to, void* __restrict__ from) override
      // {
      //   std::memcpy(to, from, sizeof(T));
      // }
  };

  constexpr bool INITABLE_TYPE = true;

  template<class T, bool has_init = false>
  class NonTrivialTypeManager : public TypeManager
  {
    public:
      void constructor(void* component) override
      {
        new (component) T{};

        if constexpr (has_init)
          reinterpret_cast<T*>(component)->init();
      }

      void constructor(void* __restrict__ component, const void* __restrict__ init_comp) override
      {
        const T& init = *reinterpret_cast<const T*>(init_comp);
        new (component) T(init);

        if constexpr (has_init)
          reinterpret_cast<T*>(component)->init();
      }
      void moveConstructor(void* __restrict__ component, void* __restrict__ init_comp) override
      {
        T& init = *reinterpret_cast<T*>(init_comp);
        new (component) T(std::move(init));
      }
      virtual
      void destructor(void* component) override
      {
        T* s =  reinterpret_cast<T*>(component);
        s->~T();
      }
      // virtual
      // void copy(void* __restrict__ to, const void* __restrict__ from) override
      // {
      //   T& _to = *reinterpret_cast<T*>(to);
      //   const T& _from = *reinterpret_cast<const T*>(from);
      //   _to = _from;
      // }

      // virtual
      // void move(void* __restrict__ to, void* __restrict__ from) override
      // {
      //   T& _to = *reinterpret_cast<string*>(to);
      //   T& _from = *reinterpret_cast<string*>(from);
      //   _to = std::move(_from);
      // }
  };

  template<class T>
  class HandlerTypeManager : public TypeManager
  {
    public:
      void constructor(void* component) override
      {
        new (component) T{};
      }

      void constructor(void* __restrict__ component, const void* __restrict__ init_comp) override
      {
        ASSERT(!"Handlers type must be used with move semantics only");
      }
      void moveConstructor(void* __restrict__ component, void* __restrict__ init_comp) override
      {
        T& init = *reinterpret_cast<T*>(init_comp);
        new (component) T(std::move(init));
      }
      virtual
      void destructor(void* component) override
      {
        T* s =  reinterpret_cast<T*>(component);
        s->~T();
      }
  };

  struct TypeMeta
  {
    size_t size;
    component_type_id_t typeId;
    const char* typeName;

    TypeManager* manager;

    uint64_t isTrivial : 1;
    uint64_t isTrivialRelocatable : 1;
  };

  class TypeMetaRegistration
  {
    public:
      TypeMetaRegistration(const TypeMeta&);

      template<class T>
      static
      void enumerate(T cb);

      static
      auto size() -> size_t
      {
        return m_List ? m_List->m_Id+1 : 0;
      }

    private:
      static TypeMetaRegistration* m_List;
      TypeMetaRegistration* m_Next;

      TypeMeta m_Meta;
      size_t m_Id;
  };

  #define DECLARE_ECS_COMPONENT(type, type_name, type_manager, is_trivial, is_trivial_relocatable) \
    ecs::TypeMetaRegistration type ## _reg{ ecs::TypeMeta{ \
      .size = sizeof(type), \
      .typeId = ecs::CompileTypeId::from<type>(), \
      .typeName = type_name, \
      .manager = new type_manager, \
      .isTrivial = is_trivial, \
      .isTrivialRelocatable = is_trivial_relocatable }}

  #define DECLARE_TRIVIAL_ECS_COMPONENT_WITH_NAME(type, name) \
          DECLARE_ECS_COMPONENT(type, name, ecs::TrivialTypeManager<type>, true, true)

  #define DECLARE_TRIVIAL_ECS_COMPONENT(type) \
          DECLARE_TRIVIAL_ECS_COMPONENT_WITH_NAME(type, #type)
  
  #define DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT(type) \
          DECLARE_ECS_COMPONENT(type, #type, ecs::NonTrivialTypeManager<type>, false, true)

  #define DECLARE_INITABLE_ECS_OBJECT_COMPONENT(type) \
          DECLARE_ECS_COMPONENT(type, #type, (ecs::NonTrivialTypeManager<type, ecs::INITABLE_TYPE>), false, true)

  class TypeMetaStorage
  {
    public:
      void init();

      auto getMeta(const component_type_id_t type_id) const -> const TypeMeta* ;
      auto getMeta(const string_view type_name) const -> const TypeMeta* ;

    private:
      eastl::vector<TypeMeta> m_Metas;
      eastl::vector_map<string_hash, component_type_id_t> m_NameToTypeId;
  };

  void init_meta_storage();
  auto get_meta_storage() -> const TypeMetaStorage&;
}

#include "type_meta.inc.hpp"