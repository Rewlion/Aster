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
      void constructor(void* __restrict__ component, const void* __restrict__ init_comp) {}
      
      virtual
      void moveConstructor(void* __restrict__ component, void* __restrict__ init_comp) {}

      virtual
      void destructor(void* component) {}

      virtual
      void copy(void* __restrict__ to, const void* __restrict__ from) {}

      virtual
      void move(void* __restrict__ to, void* __restrict__ from) {}
  };

  template<class T>
  class TrivialTypeManager : public TypeManager
  {
    public:
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

      void copy(void* __restrict__ to, const void* __restrict__ from) override
      {
        std::memcpy(to, from, sizeof(T));
      }

      void move(void* __restrict__ to, void* __restrict__ from) override
      {
        std::memcpy(to, from, sizeof(T));
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

  #define DECLARE_ECS_COMPONENT(type, type_manager, is_trivial, is_trivial_relocatable) \
    TypeMetaRegistration type ## _reg{ TypeMeta{ \
      .size = sizeof(type), \
      .typeId = CompileTypeId::from<type>(), \
      .typeName = #type, \
      .manager = new type_manager, \
      .isTrivial = is_trivial, \
      .isTrivialRelocatable = is_trivial_relocatable }}

  #define DECLARE_TRIVIAL_ECS_COMPONENT(type) \
          DECLARE_ECS_COMPONENT(type, TrivialTypeManager<type>, true, true)

  class TypeMetaStorage
  {
    public:
      void init();

      auto getMeta(const component_type_id_t type_id) const -> const TypeMeta* ;

    private:
      eastl::vector<TypeMeta> m_Metas;
      eastl::vector_map<string_hash, component_type_id_t> m_NameToTypeId;
  };

  void init_meta_storage();
  auto get_meta_storage() -> const TypeMetaStorage&;
}

#include "type_meta.inc.hpp"