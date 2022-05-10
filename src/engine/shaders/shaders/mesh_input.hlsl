#include "mesh_uniforms.hlsl"

struct StaticMeshVsInput
{
  float3 pos: POSITION0;
  float3 tan: TEXCOORD1;
  float3 bitan: TEXCOORD2;
  float2 uv: TEXCOORD0;
};
