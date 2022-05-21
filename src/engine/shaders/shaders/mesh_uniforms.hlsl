#include "uniforms_common.hlsl"

struct PerStaticMeshUniform
{
  float4x4 modelTm;
  float4x4 normalTm;
};

#define INIT_STATIC_MESH_UNIFORMS()\
  Texture2D<float4>                    albedo_tex:          register(t0, DSET_PER_MATERIAL);\
  Texture2D<float4>                    metal_roughness_tex: register(t1, DSET_PER_MATERIAL);\
  Texture2D<float4>                    normal_tex:          register(t2, DSET_PER_MATERIAL);\
                                                                                            \
  ConstantBuffer<PerStaticMeshUniform> model_uniforms:      register(b0, DSET_PER_MODEL);
