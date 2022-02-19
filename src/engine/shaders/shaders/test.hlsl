struct PushConstants
{
  float4 color;
};

[[vk::push_constant]] PushConstants consts;

struct VsInput
{
  float2 pos: POSITION0;
};

struct VsOutput
{
  float4 pos: SV_Position;
};

VsOutput vs_main(VsInput input)
{
  VsOutput output;
  output.pos = float4(input.pos, 0.0, 1.0);

  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  return consts.color;
}
