#include "uniforms_common.hlsl"

struct FrameUniforms
{
  float4x4 viewProj;
};

#define INIT_FRAME_UNIFORMS() \
  ConstantBuffer<FrameUniforms> frame_uniforms: register(b0, DSET_PER_FRAME);\
  sampler                       model_sampler:  register(s1, DSET_PER_FRAME);
