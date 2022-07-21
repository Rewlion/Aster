#pragma once

#include <engine/gapi/resources.h>

namespace tfx
{
  struct RenderState
  {
    gapi::PrimitiveTopology topology;
    gapi::DepthStencilStateDescription depthStencil;
    gapi::BlendState blending;

    bool operator==(const RenderState& rvl) const
    {
      return std::tie(topology, depthStencil, blending) ==
             std::tie(rvl.topology, rvl.depthStencil, rvl.blending);
    }
  };
}
