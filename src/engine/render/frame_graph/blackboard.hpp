#pragma once

#include <engine/assert.h>

#include <EASTL/hash_map.h>

#include <any>

namespace fg
{
  class Blackboard
  {
    struct TypeIdHelper
    {
      template<class T>
      static size_t getId();
      private:
        static size_t lastId;
    };
    public:
      template<class T>
      T& add();

      template<class T>
      T& get();

    private:
      eastl::hash_map<size_t, std::any> m_Components;
  };
}

#include "blackboard.inc"
