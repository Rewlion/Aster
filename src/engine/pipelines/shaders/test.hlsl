
struct VsInput
{
  float4 pos: POSITION0;
};

struct VsOutput
{
  float4 pos: POSITION0;
};

VsOutput vs_main(VsInput input)
{
  VsOutput output;
  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  return float4(0,0,0,0);
}
