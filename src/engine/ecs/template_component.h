#pragma once

#include "core_types.h"
#include "type_meta.h"
#include "hash.h"

namespace ecs
{
  class TemplateComponent
  {
    friend class Registry;
    friend class TemplateComponentsMap;
    friend class EntityComponents;

    public:
      TemplateComponent() = default;

      template<class T>
      void operator=(T&& v) &;

      auto getMeta() const -> const TypeMeta*;

      auto getNameHash() const -> name_hash_t;

    private:
      TemplateComponent(const name_hash_t hash);

      void destroy();

      template<class T>
      static
      auto isBoxedType() -> bool;

      static
      auto isBoxedType(const size_t size) -> bool;

      auto getData() const -> const void*;

      union
      {
        void* ptr;
        uint64_t rawValue;
      } as;

      component_type_id_t m_TypeId;
      name_hash_t m_NameHash;
  };

  class TemplateComponentsMap
  {
    public:
      auto operator[] (const string_view name) -> TemplateComponent&;

      auto getComponent(const name_hash_t name) const -> const TemplateComponent&;

      auto getMap() const -> const eastl::vector_map<name_hash_t, TemplateComponent>&;

      auto getComponentName(const name_hash_t name) const -> string_view;

      ~TemplateComponentsMap();

    private:
      eastl::vector_map<name_hash_t, TemplateComponent> m_Components;
      eastl::vector_map<name_hash_t, string> m_Names;
  };
}

#include "template_component.inc.hpp"
