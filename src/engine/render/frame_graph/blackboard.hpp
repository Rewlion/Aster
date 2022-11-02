#pragma once

#include <engine/algorithm/hash.h>
#include <engine/assert.h>

#include <EASTL/hash_map.h>

#include <any>

namespace fg
{
  class Blackboard
  {
    typedef size_t ComponentTypeId;
    typedef string_hash ComponentId;

    struct TypeIdHelper
    {
      template<class T>
      static ComponentTypeId getId();
      private:
        static ComponentTypeId lastId;
    };
    public:
      inline ComponentId getComponentId(const std::string_view name) const
      {
        return str_hash(name);
      }

      template<class T>
      T& add(T&& value = T{}, const std::string_view name = "");

      template<class T>
      T& get(const std::string_view name = "");

    private:
      eastl::hash_map<ComponentTypeId, eastl::hash_map<ComponentId, std::any>> m_Components;
  };
}

#include "blackboard.inc"
