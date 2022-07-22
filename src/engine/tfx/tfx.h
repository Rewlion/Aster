#pragma once

#include <engine/gapi/resources.h>

namespace tfx
{
  struct RenderState
  {
    gapi::VertexInputDescription ia;
    gapi::PrimitiveTopology topology;
    gapi::DepthStencilStateDescription depthStencil;
    gapi::BlendState blending;

    bool operator==(const RenderState& rvl) const
    {
      return std::tie(topology, ia, depthStencil, blending) ==
             std::tie(rvl.topology, rvl.ia, rvl.depthStencil, rvl.blending);
    }
  };
}
