#pragma once

#include "eid.h"
#include "types.h"

#include <engine/algorithm/hash.h>
#include <engine/types.h>
#include <engine/assert.h>

#include <EASTL/vector_map.h>

struct Component
{
  size_t size = 0;
  size_t blockOffset = 0;
  component_type_id typeId = INVALID_COMPONENT_TYPE_ID;
};

typedef eastl::vector_map<component_name_id, Component> ComponentMap;

struct ComponentDescription
{
  string name = "";
  component_name_id id = INVALID_HASH;
  size_t size = 0;
  component_type_id type = INVALID_COMPONENT_TYPE_ID;
};

#define DESCRIBE_COMPONENT(componentName, componentType)\
  ComponentDescription{\
    .name = componentName,\
    .id = str_hash(componentName),\
    .size = sizeof(componentType),\
    .type = str_hash(#componentType)\
  }

struct ComponentMeta
{
  size_t size = 0;
  component_type_id typeId = INVALID_COMPONENT_TYPE_ID;
};

#define DECLARE_COMPONENT_META(type)\
  constexpr ComponentMeta type ## _ComponentMeta = ComponentMeta\
  {\
   .size = sizeof(type),\
   .typeId = str_hash(#type)\
  };

DECLARE_COMPONENT_META(int)
DECLARE_COMPONENT_META(int2)
DECLARE_COMPONENT_META(int3)
DECLARE_COMPONENT_META(int4)
DECLARE_COMPONENT_META(float)
DECLARE_COMPONENT_META(float2)
DECLARE_COMPONENT_META(float3)
DECLARE_COMPONENT_META(float4)
DECLARE_COMPONENT_META(string)
DECLARE_COMPONENT_META(EntityId)

inline void component_initializer(uint8_t* comp_data, const component_type_id typeId, uint8_t* value)
{
  #define declare_initialization(type)\
  {\
    case type ## _ComponentMeta.typeId:\
    type* compData = new(comp_data) type;\
    type* compValue = reinterpret_cast<type*>(value);\
    *compData = std::move(*compValue);\
    break;\
  }

  switch(typeId)
  {
    declare_initialization(int)
    declare_initialization(int2)
    declare_initialization(int3)
    declare_initialization(int4)
    declare_initialization(float)
    declare_initialization(float2)
    declare_initialization(float3)
    declare_initialization(float4)
    declare_initialization(string)
    declare_initialization(EntityId)
    default:
      ASSERT(!"unknown type");
  }

  #undef declare_initialization
}

inline void component_default_initializer(uint8_t* data, const component_type_id typeId)
{
  #define declare_initialization(type, ...)\
    case type ## _ComponentMeta.typeId:\
    {\
      type default_value = type(__VA_ARGS__);\
      component_initializer(data, typeId, reinterpret_cast<uint8_t*>(&default_value));\
      break;\
    }

  switch(typeId)
  {
    declare_initialization(int, 0)
    declare_initialization(int2, 0,0)
    declare_initialization(int3, 0,0,0)
    declare_initialization(int4, 0,0,0,0)
    declare_initialization(float, 0.0f)
    declare_initialization(float2, 0.0f, 0.0f)
    declare_initialization(float3, 0.0f, 0.0f, 0.0f)
    declare_initialization(float4, 0.0f, 0.0f, 0.0f, 0.0f)
    declare_initialization(string, string{""})
    declare_initialization(EntityId, EntityId{})
    default:
      ASSERT(!"unknown type");
  }

  #undef declare_initialization
}

template<class T> inline component_type_id get_component_type_id() { return INVALID_COMPONENT_TYPE_ID; }
#define declare_component_type_id_getter(type)\
  template<> inline component_type_id get_component_type_id<type>() { return type ## _ComponentMeta.typeId; }

declare_component_type_id_getter(int)
declare_component_type_id_getter(int2)
declare_component_type_id_getter(int3)
declare_component_type_id_getter(int4)
declare_component_type_id_getter(float)
declare_component_type_id_getter(float2)
declare_component_type_id_getter(float3)
declare_component_type_id_getter(float4)
declare_component_type_id_getter(string)
declare_component_type_id_getter(EntityId)
#undef declare_component_type_id_getter

inline void component_destructor(uint8_t* data, component_type_id typeId)
{
  #define NO_DESTRUCTOR(type) case type ## _ComponentMeta.typeId: break;
  #define DESTRUCTOR(type) case type ## _ComponentMeta.typeId: reinterpret_cast<type*>(data)->~type();

  switch(typeId)
  {
    NO_DESTRUCTOR(int)
    NO_DESTRUCTOR(int2)
    NO_DESTRUCTOR(int3)
    NO_DESTRUCTOR(int4)
    NO_DESTRUCTOR(float)
    NO_DESTRUCTOR(float2)
    NO_DESTRUCTOR(float3)
    NO_DESTRUCTOR(float4)
    NO_DESTRUCTOR(EntityId)
    DESTRUCTOR(string)

    default:
      ASSERT(!"unknown type");
  }

  #undef NO_DESTRUCTOR
  #undef DESTRUCTOR
}
