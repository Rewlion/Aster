#pragma once

#include <engine/render/frame_graph/handles.h>

namespace Engine::Render::blackboard
{
  struct Gbuffer
  {
    fg::VirtualResourceHandle depth;
    fg::VirtualResourceHandle albedo;
    fg::VirtualResourceHandle normal;
    fg::VirtualResourceHandle metalRoughness;
    fg::VirtualResourceHandle worldPos;
  };
}
