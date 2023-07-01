#pragma once

#include <engine/types.h>

namespace Engine
{
  struct CameraData
  {
    float3 pos;
    mat4 view;
    mat4 proj;
    float zNear;
    float zFar;
  };

  CameraData get_camera(const float aspect);
}
