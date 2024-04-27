#include "gi_state.h"

#include <engine/ecs/type_meta.h>

void GIOnSurfels::init()
{
  initialized = false;
}

DECLARE_INITABLE_ECS_OBJECT_COMPONENT(GIOnSurfels);
