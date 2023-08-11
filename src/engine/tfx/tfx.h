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
      gapi::TextureHandle,
      gapi::BufferHandler,
      gapi::SamplerHandler> Param;

  struct RenderState
  {
    gapi::VertexInputDescription ia;
    gapi::CullMode cullMode = gapi::CullMode::CW;
    gapi::PrimitiveTopology topology = gapi::PrimitiveTopology::TriangleList;
    gapi::DepthStencilStateDescription depthStencil;
    gapi::BlendState blending;
    uint32_t tsInputControlPatchCount = 0;

    bool operator==(const RenderState& rvl) const
    {
      return std::tie(topology, ia, depthStencil, blending, tsInputControlPatchCount) ==
             std::tie(rvl.topology, rvl.ia, rvl.depthStencil, rvl.blending, rvl.tsInputControlPatchCount);
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

  void load_materials_bin(string_view file);

  void reload_materials_bin(string_view file);

  void set_extern(const string& name, const Param& p);

  void set_channel(const string& name, const Param& p);

  void activate_scope(string_view name, gapi::CmdEncoder& cmd_encoder);

  void activate_technique(string_view name, gapi::CmdEncoder& cmd_encoder);
}
