#pragma once

#include "eid.h"
#include "types.h"

#include <engine/algorithm/hash.h>
#include <engine/types.h>
#include <engine/assert.h>

#include <EASTL/vector_map.h>

namespace Engine::ECS
{
  struct TemplateComponentDescription
  {
    string name;
    string type;
  };

  struct ArchetypeComponent
  {
    component_name_id nameId;
    component_type_id typeId;
    size_t blockOffset;
  };

  using TemplateToArchetypeMap = eastl::vector_map<template_name_id, archetype_id>;
  using CompToPlacementMap = eastl::vector_map<component_type_id, type_placement_id>;

  class ComponentTypeId
  {
    public:
      template<class T>
      static component_type_id from()
      {
        return fromInternal<typename std::remove_cvref<T>::type>();
      }
    private:
      template<class T>
      static component_type_id fromInternal()
      {
        static component_type_id id = m_LastId++;
        return id;
      }
    private:
      static component_type_id m_LastId;
  };

  class TypeManager
  {
    public:
      virtual void constructor(void* component) {}
      virtual void destructor(void* component) {}
      virtual void copy(void* __restrict__ to, const void* __restrict__ from) {}
      virtual void move(void* __restrict__ to, void* __restrict__ from) {}
  };

  template<class T>
  class TrivialTypeManager : public TypeManager
  {
    public:
      virtual void constructor(void* component) override
      {
        std::memset(component, 0, sizeof(T));
      }
      virtual void destructor(void* component) override
      {
      }

      virtual void copy(void* __restrict__ to, const void* __restrict__ from) override
      {
        std::memcpy(to, from, sizeof(T));
      }

      virtual void move(void* __restrict__ to, void* __restrict__ from) override
      {
        std::memcpy(to, from, sizeof(T));
      }
  };

  struct ComponentMeta
  {
    size_t size;
    component_type_id typeId;
    const char* typeName;

    TypeManager* manager;

    uint64_t isTrivial : 1;
    uint64_t isTrivialRelocatable : 1;
  };

  class ComponentMetaRegistration
  {
    public:
      ComponentMetaRegistration(const ComponentMeta&);

      template<class T>
      static void enumerate(T cb)
      {
        const ComponentMetaRegistration* p = m_List;
        while(p)
        {
          cb(p->m_Meta);
          p = p->m_Next;
        }
      }

      static size_t size()
      {
        return m_List ? m_List->m_Id+1 : 0;
      }

    private:
      static ComponentMetaRegistration* m_List;
      ComponentMetaRegistration* m_Next;

      ComponentMeta m_Meta;
      size_t m_Id;
  };

  #define DECLARE_ECS_COMPONENT(type, type_manager, is_trivial, is_trivial_relocatable) \
    ComponentMetaRegistration type ## _reg{ ComponentMeta{ \
      .size = sizeof(type), \
      .typeId = ComponentTypeId::from<type>(), \
      .typeName = #type, \
      .manager = new type_manager, \
      .isTrivial = is_trivial, \
      .isTrivialRelocatable = is_trivial_relocatable }}

  #define DECLARE_TRIVIAL_ECS_COMPONENT(type) \
          DECLARE_ECS_COMPONENT(type, TrivialTypeManager<type>, true, true)

  class ComponentMetaStorage
  {
    public:
      void init();

      const ComponentMeta& getMeta(const string_view type) const;
      const ComponentMeta& getMeta(const component_type_id type_id) const;

    private:
      eastl::vector<ComponentMeta> m_Metas;
      eastl::vector_map<string_hash, component_type_id> m_NameToTypeId;
  };

  void init_meta_storage();
  const ComponentMetaStorage& get_meta_storage();
}