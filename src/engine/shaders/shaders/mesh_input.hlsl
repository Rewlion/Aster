#include "mesh_uniforms.hlsl"

struct StaticMeshVsInput
{
  float3 pos: POSITION0;
  float3 tan: TEXCOORD1;
  float3 normal: TEXCOORD2;
  float2 uv: TEXCOORD0;
};

#define INIT_MODEL_SAMPLING()\
\
float4 sample_model_albedo(float2 uv)\
{\
  return albedo_tex.Sample(model_sampler, uv).rgba;\
}\
\
void sample_model_metal_roughness(inout float metal, inout float roughness, float2 uv)\
{\
  float3 texture = metal_roughness_tex.Sample(model_sampler, uv).rgb;\
  metal = texture.b;\
  roughness = texture.g;\
}\
\
float3 sample_model_normal(float3 N)\
{\
  return N;\
}