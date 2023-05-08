#pragma once

#include <engine/types.h>

namespace Engine
{
  struct CameraData
  {
    float3 pos;
    mat4 viewProj;
  };

  CameraData get_camera(const float aspect);
}
