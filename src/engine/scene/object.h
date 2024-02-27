#pragma once

#include <engine/types.h>

namespace Engine
{
  struct SceneObject
  {
    float3 pos = float3(0,0,0);
    float3 scale = float3(1, 1, 1);
    float3 rot = float3(0, 0, 0);
    string model;
  };
}
