#pragma once

#include "eid.h"
#include "types.h"
#include "string_hash.h"

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
  String name = "";
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
DECLARE_COMPONENT_META(Int2)
DECLARE_COMPONENT_META(Int3)
DECLARE_COMPONENT_META(Int4)
DECLARE_COMPONENT_META(float)
DECLARE_COMPONENT_META(Float2)
DECLARE_COMPONENT_META(Float3)
DECLARE_COMPONENT_META(Float4)
DECLARE_COMPONENT_META(String)
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
    declare_initialization(Int2)
    declare_initialization(Int3)
    declare_initialization(Int4)
    declare_initialization(float)
    declare_initialization(Float2)
    declare_initialization(Float3)
    declare_initialization(Float4)
    declare_initialization(String)
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
    declare_initialization(Int2, 0,0)
    declare_initialization(Int3, 0,0,0)
    declare_initialization(Int4, 0,0,0,0)
    declare_initialization(float, 0.0f)
    declare_initialization(Float2, 0.0f, 0.0f)
    declare_initialization(Float3, 0.0f, 0.0f, 0.0f)
    declare_initialization(Float4, 0.0f, 0.0f, 0.0f, 0.0f)
    declare_initialization(String, String{""})
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
declare_component_type_id_getter(Int2)
declare_component_type_id_getter(Int3)
declare_component_type_id_getter(Int4)
declare_component_type_id_getter(float)
declare_component_type_id_getter(Float2)
declare_component_type_id_getter(Float3)
declare_component_type_id_getter(Float4)
declare_component_type_id_getter(String)
declare_component_type_id_getter(EntityId)
#undef declare_component_type_id_getter

inline void component_destructor(uint8_t* data, component_type_id typeId)
{
  #define NO_DESTRUCTOR(type) case type ## _ComponentMeta.typeId: break;
  #define DESTRUCTOR(type) case type ## _ComponentMeta.typeId: reinterpret_cast<type*>(data)->~type();

  switch(typeId)
  {
    NO_DESTRUCTOR(int)
    NO_DESTRUCTOR(Int2)
    NO_DESTRUCTOR(Int3)
    NO_DESTRUCTOR(Int4)
    NO_DESTRUCTOR(float)
    NO_DESTRUCTOR(Float2)
    NO_DESTRUCTOR(Float3)
    NO_DESTRUCTOR(Float4)
    NO_DESTRUCTOR(EntityId)
    DESTRUCTOR(String)

    default:
      ASSERT(!"unknown type");
  }

  #undef NO_DESTRUCTOR
  #undef DESTRUCTOR
}
