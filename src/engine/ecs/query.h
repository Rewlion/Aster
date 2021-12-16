#pragma once

#include "types.h"

#include <EASTL/fixed_vector.h>

#include <EASTL/functional.h>

struct QueryComponentDescription
{
  component_type_id typeId = INVALID_COMPONENT_TYPE_ID;
  component_name_id nameId = INVALID_COMPONENT_NAME_ID;
};

using QueryCb = void(*)(class ComponentsAccessor&);
using EventQueryCb = void(*)(struct Event*, class ComponentsAccessor&);
using DirectQueryCb = eastl::function<void(class ComponentsAccessor&)>;
using QueryComponents = eastl::fixed_vector<QueryComponentDescription, 16, true>;
using DesiredArchetypes = eastl::fixed_vector<archetype_id, 4, true>;

struct QueryDescription
{
  QueryCb cb;
  QueryComponents components;
  EventQueryCb eventCb;
  event_hash_name event = INVALID_HASH;
};

struct RegisteredQueryInfo
{
  QueryCb cb;
  DesiredArchetypes archetypes;
};

struct RegisteredEventQueryInfo
{
  EventQueryCb eventCb;
  DesiredArchetypes archetypes;
};

struct DirectQueryDescription
{
  QueryComponents components;
};

struct DirectQuery
{
  QueryComponents components;
  DesiredArchetypes desiredArchetypes;
};

#define DESCRIBE_QUERY_COMPONENT(componentName, componentType)\
  QueryComponentDescription\
  {\
    .typeId = str_hash(#componentType),\
    .nameId = str_hash(componentName)\
  }
