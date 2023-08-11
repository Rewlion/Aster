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

  auto get_camera(const float aspect, const float3 jitter) -> CameraData;
  auto get_camera_pos() -> float3;
}
