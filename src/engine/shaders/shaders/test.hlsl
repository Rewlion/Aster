struct PushConstants
{
  float4x4 mvp;
};

[[vk::push_constant]] PushConstants consts;

struct VsInput
{
  float3 pos: POSITION0;
  float2 uv: TEXCOORD0;
  float3 tan: TEXCOORD1;
  float3 bitan: TEXCOORD2;
};

struct VsOutput
{
  float4 pos: SV_Position;
  float2 uv: TEXCOORD0;
};

Texture2D<float4> test_texture: register(t0, space0);
sampler test_sampler: register(s1, space0);

VsOutput vs_main(VsInput input)
{
  VsOutput output;
  output.pos = mul(consts.mvp, float4(input.pos, 1.0));
  output.uv = input.uv;

  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  float4 a = test_texture.Sample(test_sampler, input.uv);
  return a;
}
