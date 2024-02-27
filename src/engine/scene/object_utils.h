#pragma once

#include <engine/types.h>

namespace Engine
{
  struct SceneObject;
  auto calc_object_to_world_tm(const SceneObject&) -> float4x4;
}