#ifndef COLOR_RAMP_HLSL
#define COLOR_RAMP_HLSL

float3 linearColorRamp(float3 a, float3 b, float3 c, float t)
{
  t = saturate(t);

  float abT = smoothstep(0.0, 1.0, t/0.5);
  float bcT = smoothstep(0.0, 1.0, (t-0.5)/0.5);

  return lerp(
          lerp(a, b, abT),
          c,
          bcT);
}

float3 rgbColorRamp(float t)
{
  return linearColorRamp(
    float3(1,0,0),
    float3(0,1,0),
    float3(0,0,1),
    t
  );
}

float3 heatmapColorRamp(float t)
{
  return linearColorRamp(
    float3(77, 31, 99) / 255.0,
    float3(203, 67, 100) / 255.0,
    float3(237, 165, 122) / 255.0,
    t
  );
}

#endif