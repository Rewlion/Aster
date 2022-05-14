#include "frame_uniforms.hlsl"
#include "mesh_input.hlsl"

INIT_FRAME_UNIFORMS()
INIT_STATIC_MESH_UNIFORMS()

struct VsOutput
{
  float4 pos: SV_Position;
  float2 uv: TEXCOORD0;
  float3 normal: TEXCOORD1;
};

VsOutput vs_main(StaticMeshVsInput input)
{
  float4x4 mvp = frame_uniforms.viewProj;
  mvp = mul(mvp, model_uniforms.modelTm);

  VsOutput output;
  output.pos = mul(mvp, float4(input.pos, 1.0));
  output.uv = input.uv;
  output.normal = normalize(mul(model_uniforms.normalTm, float4(input.normal, 0.0)).xyz);

  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  float t = frame_uniforms.secSinceStart;
  float3 sunDir = normalize(float3(cos(t), -0.3f, sin(t)));

  float4 albedo = albedo_tex.Sample(model_sampler, input.uv);
  float4 ambient = 0.1 * albedo;
  float4 diffuse = clamp(dot(-sunDir, input.normal), 0.0, 1.0) * albedo;

  return diffuse + ambient;
}
