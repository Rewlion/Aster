#pragma once

#include <engine/ecs/ecs_events.h>
#include <engine/gapi/resource_wrappers.h>

namespace gapi
{
  class CmdEncoder;
}
namespace Engine
{
  ECS_EVENT_BEGIN(OnBeforeRender)
    ECS_EVENT_FIELD(gapi::CmdEncoder* encoder)
  ECS_EVENT_END()
 
  ECS_EVENT_BEGIN(DebugOnBeforePresent)
    ECS_EVENT_FIELD(gapi::CmdEncoder* encoder)
    ECS_EVENT_FIELD(gapi::TextureViewWithState* backbuffer)
  ECS_EVENT_END()

  ECS_EVENT_BEGIN(OnFrameGraphInit)
  ECS_EVENT_END()
}
