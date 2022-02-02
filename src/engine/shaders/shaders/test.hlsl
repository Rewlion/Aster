
struct VsOutput
{
  float4 pos: SV_Position;
};

VsOutput vs_main(uint id: SV_VertexID)
{
  float2 vertices[4] = {
    float2(0,0),
    float2(0,1),
    float2(1,0),
    float2(1,1),
  };

  VsOutput output;
  output.pos = float4(vertices[id], 0.0, 1.0);

  return output;
}

float4 ps_main(VsOutput input): SV_Target0
{
  return float4(255,255,0,0);
}
