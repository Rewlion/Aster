#include "frame_uniforms.hlsl"
#include "mesh_input.hlsl"

INIT_FRAME_UNIFORMS()
INIT_STATIC_MESH_UNIFORMS()
INIT_MODEL_SAMPLING()

#include "gbuffer.hlsl"

struct VsOutput
{
  float4 pos: SV_Position;
  float3 worldPos: POSITION0;
  float2 uv: TEXCOORD0;
  float3 normal: TEXCOORD1;
};

VsOutput vs_main(StaticMeshVsInput input)
{
  float4x4 mvp = frame_uniforms.viewProj;
  mvp = mul(mvp, model_uniforms.modelTm);

  VsOutput output;
  output.pos = mul(mvp, float4(input.pos, 1.0));
  output.worldPos = mul(model_uniforms.modelTm, float4(input.pos, 1.0)).xyz;
  output.uv = input.uv;
  output.normal = normalize(mul(model_uniforms.normalTm, float4(input.normal, 0.0)).xyz);

  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  float4 albedo = sample_model_albedo(input.uv);
  float metal;
  float roughness;
  sample_model_metal_roughness(metal, roughness, input.uv);

  Gbuffer gbuffer;
  init_albedo(gbuffer, albedo);
  init_metal(gbuffer, metal);
  init_roughness(gbuffer, roughness);
  init_normal(gbuffer, input.normal);
  init_worldPos(gbuffer, input.worldPos);

  return encode_gbuffer(gbuffer);
}
