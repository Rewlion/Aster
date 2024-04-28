#ifndef COLOR_SPACES_HLSL
#define COLOR_SPACES_HLSL

float3 RGB2YCbCR(float3 rgb)
{
  float3 res;
  res.x = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
  res.y = -0.168736 * rgb.r - 0.331264 * rgb.g + 0.5 * rgb.b;
  res.z = 0.5 * rgb.r - 0.418688 * rgb.g - 0.081312 * rgb.b;
  return res * float3(219, 224, 224) + float3(16, 128, 128);
}

float3 YCbCr2RGB(float3 ycbcr)
{
  float3 res;
  res.x = 298.082 / 256.0 * ycbcr.x + 408.583 * ycbcr.z / 256.0 - 222.921;
  res.y = 298.082 / 256.0 * ycbcr.x - 100.291 / 256.0 * ycbcr.y - 208.120 / 256.0 * ycbcr.z+ 135.576;
  res.z = 298.082 / 256.0 * ycbcr.x + 516.412 / 256.0 * ycbcr.y - 276.836;
  return res / 256.0;
}

float luminance(float3 rgb)
{
  return max(dot(rgb, float3(0.299, 0.587, 0.144)), 0.001);
}

float luminance_ub(float3 rgb)
{
  return dot(rgb, float3(0.299, 0.587, 0.144));
}
#endif