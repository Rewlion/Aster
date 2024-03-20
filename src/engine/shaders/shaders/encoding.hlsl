#ifndef ENCODING_HLSL
#define ENCODING_HLSL

///////////////////////////////////////////////////////////////////
// [A Survey of Efficient Representations for Independent Unit Vectors]
// https://jcgt.org/published/0003/02/01/paper.pdf

// Assume normalized input on +Z hemisphere.
// Output is on [-1, 1].
float2 float32x3_to_hemioct(in float3 v)
{
  // Project the hemisphere onto the hemi-octahedron,
  // and then into the xy plane
  float2 p = v.xy * (1.0 / (abs(v.x) + abs(v.y) + v.z));
  // Rotate and scale the center diamond to the unit square
  return float2(p.x + p.y, p.x - p.y);
}

float3 hemioct_to_float32x3(float2 e)
{
  // Rotate and scale the unit square back to the center diamond
  float2 temp = float2(e.x + e.y, e.x - e.y) * 0.5;
  float3 v = float3(temp, 1.0 - abs(temp.x) - abs(temp.y));
  return normalize(v);
}
///////////////////////////////////////////////////////////////////

#endif