#pragma once

#include <engine/gapi/resources.h>

namespace gapi
{
  class CmdEncoder;
}

namespace tfx
{
  typedef std::variant<
      int,int2,int3,int4,
      float,float2,float3,float4,
      float4x4,
      gapi::TextureHandler,
      gapi::BufferHandler> Param;

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

  struct ParamDescription
  {
    const string name;
    Param value;
  };

  struct Material
  {
    string technique;
    eastl::vector<ParamDescription> params;
  };

  void load_materials_bin(const string& file);

  void set_extern(const string& name, const Param& p);

  void set_channel(const string& name, const Param& p);

  void activate_scope(const string& name, gapi::CmdEncoder* cmd_encoder);

  void activate_technique(const string& name, gapi::CmdEncoder* cmd_encoder);
}
