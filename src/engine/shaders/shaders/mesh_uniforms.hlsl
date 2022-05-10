#include "uniforms_common.hlsl"

struct PerStaticMeshUniform
{
  float4x4 modelTm;
};

#define INIT_STATIC_MESH_UNIFORMS()\
  Texture2D<float4>                    albedo_tex:     register(t0, DSET_PER_MATERIAL);\
  ConstantBuffer<PerStaticMeshUniform> model_uniforms: register(b0, DSET_PER_MODEL);
