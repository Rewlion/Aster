#include "frame_uniforms.hlsl"
#include "mesh_input.hlsl"

INIT_FRAME_UNIFORMS()
INIT_STATIC_MESH_UNIFORMS()

struct VsOutput
{
  float4 pos: SV_Position;
  float2 uv: TEXCOORD0;
};

VsOutput vs_main(StaticMeshVsInput input)
{
  float4x4 mvp = frame_uniforms.viewProj;
  mvp = mul(mvp, model_uniforms.modelTm);

  VsOutput output;
  output.pos = mul(mvp, float4(input.pos, 1.0));
  output.uv = input.uv;

  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  return albedo_tex.Sample(model_sampler, input.uv);
}
